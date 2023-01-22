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