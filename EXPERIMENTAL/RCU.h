#pragma once
#include <atomic>
#include <array>

#include "Typenames.h"
#include <ManifestGLUtility/DebugLogger.h>
#include <EXPERIMENTAL/EXPERIMENTAL_RUNTIME_DATA_STRUCTURES.h>

namespace Manifest_Experimental
{
	using Generation = MFu32;
	constexpr Generation MAX_RCU_GENERATION{ 2 };
	constexpr Generation FREE_GENERATION{ UINT32_MAX };
	constexpr MFu32 RCU_MODULO{ MAX_RCU_GENERATION - 1 };
		

	template<typename T, typename Deleter>
	struct TableRCU
	{
		template<typename T>
		struct DataGeneration
		{			
			struct Data
			{
				Data(const DataGeneration&) = delete;
				Data(DataGeneration&&) = delete;				
				Data(T* _data)
					:data{ _data } {};
				~Data() {};
				T* data;
			};		
			struct ReadFlag
			{
				std::atomic<MFbool> isReading;
				char padding[64 - sizeof(isReading)];
			};
			DataGeneration(const DataGeneration&) = delete;
			DataGeneration(DataGeneration&&) = delete;
			//allows dummy data to be generated
			DataGeneration(T* data = new T)
				:protectedData{ data }{};
			~DataGeneration() {};
			Data protectedData;
			ReadFlag* readFlags;
			char padding[64 - sizeof(Data) - sizeof(ReadFlag*)];
		};
		std::array<DataGeneration<T>, MAX_RCU_GENERATION> doubleBuffer;
		TableRCU(const MFsize _maxReaders)
			: maxReaders(_maxReaders), registeredReaders{0}
		{
			for (auto& buffer : doubleBuffer)
			{
				buffer.readFlags = new DataGeneration<T>::ReadFlag[maxReaders];				
				memset(buffer.readFlags, 0, sizeof(DataGeneration<T>::ReadFlag)*maxReaders);
			}
			
		}
		Generation lock(const MFu32& readerId)
		{
			Generation currentGeneration = globalGeneration.load(std::memory_order_relaxed);
			MFsize generationIndex = currentGeneration & RCU_MODULO;
			doubleBuffer[generationIndex].readFlags[readerId].isReading.store(true, std::memory_order_release);
			Generation oldGeneration = currentGeneration;
			while (oldGeneration != (currentGeneration = globalGeneration.load(std::memory_order_acquire)))
			{
				doubleBuffer[generationIndex].readFlags[readerId].isReading.store(false, std::memory_order_relaxed);
				generationIndex = currentGeneration & RCU_MODULO;
				doubleBuffer[generationIndex].readFlags[readerId].isReading.store(true, std::memory_order_release);
				oldGeneration = currentGeneration;
			}

			return currentGeneration;
		}
		void unlock(const Generation& generation, const MFu32& readerId)
		{
			MFsize generationIndex = generation & RCU_MODULO;	
			doubleBuffer[generationIndex].readFlags[readerId].isReading.store(false, std::memory_order_relaxed);
		}
		void sync(T* update)
		{
			Generation oldGeneration = globalGeneration.load(std::memory_order_relaxed);
			MFsize generationIndex = (oldGeneration+1) & RCU_MODULO;
			doubleBuffer[generationIndex].protectedData.data = update;
			globalGeneration.store(oldGeneration + 1, std::memory_order_release);
			for (auto reader{ 0 }; reader < registeredReaders.load(std::memory_order_relaxed); ++reader)
				while (doubleBuffer[generationIndex].readFlags[reader].isReading.load(std::memory_order_acquire));
			deleter(doubleBuffer[generationIndex].protectedData.data);
		}
		MFu32 RegisterReader()
		{
			assert(registeredReaders.load(std::memory_order_seq_cst) + 1 < maxReaders);
			return registeredReaders.fetch_add(1, std::memory_order_relaxed);
		}
		const MFsize maxReaders;
		std::atomic<Generation> globalGeneration;
		std::atomic<MFu32> registeredReaders;		
	};

	template<typename T, typename Deleter>
	struct RCU
	{		
		//referenced counted data structure that protects the shared resource
		template<typename T>
		struct DataGeneration
		{
			//cannot move or copy DataGeneration
			DataGeneration(const DataGeneration&) = delete;
			DataGeneration(DataGeneration&&) = delete;			
			DataGeneration(T* _data = new T)//allows dummy data to be generated
				:data{ _data }, readers{ 0 } {};
			~DataGeneration() {};			
			std::atomic<MFu32> readers;
			T* data;				
			char pad[48]{0};//pads for 64 byte cache
		};
		//cannot move or copy RCU
		RCU(const RCU&) = delete;
		RCU(const RCU&&) = delete;
		RCU() : deleter{ Deleter{} } {};
		//Reader lock
		Generation rcu_read_lock()
		{
			//use current generation as guess
			Generation currentGeneration = globalGeneration.load(std::memory_order_relaxed);
			Generation oldGeneration = currentGeneration;
			//find the slot the generation would be written to 
			auto generationIndex = currentGeneration & RCU_MODULO;
			//add read attempt to the slot			
			dataGenerations[generationIndex].readers.fetch_add(1, std::memory_order_relaxed);
			//old==current; geneartion secured for reading
			 while (oldGeneration != (currentGeneration = globalGeneration.load(std::memory_order_acquire)))
			{
				//update old generation
				oldGeneration = currentGeneration;
				//remove bad read attempt
				dataGenerations[generationIndex].readers.fetch_sub(1, std::memory_order_release);	
				//make new read attempt
				generationIndex = currentGeneration & RCU_MODULO;
				dataGenerations[generationIndex].readers.fetch_add(1, std::memory_order_release);				
			}
			//return good read generation - guaranteed this generation will remain alive for reader
			return currentGeneration;
		};
		//reader unlock
		void rcu_read_unlock(const Generation& readGeneration)
		{
			//remove read from preserved generation
			auto generationIndex = readGeneration & RCU_MODULO;
			//unblock writer attempting to clean up generation
			dataGenerations[generationIndex].readers.fetch_sub(1, std::memory_order_release);
		};
		//writer update 
		void synchronize_rcu(T* update)
		{
			//find next slot for writing
			Generation oldGeneration = globalGeneration.load(std::memory_order_relaxed) ;
			auto generationIndex = (oldGeneration +1) & RCU_MODULO;
			//write data for future readers
			dataGenerations[generationIndex].data = update;
			//atomically update generation
			globalGeneration.store(oldGeneration +1, std::memory_order_release);
			generationIndex = oldGeneration & RCU_MODULO;
			//wait on readers to leave		
			while (dataGenerations[generationIndex].readers.load(std::memory_order_acquire));			
			//release previous data
			deleter(dataGenerations[generationIndex].data);
		};

		const Deleter deleter;
		std::array<DataGeneration<T>, MAX_RCU_GENERATION> dataGenerations;
		std::atomic<Generation> globalGeneration;		
	};
}