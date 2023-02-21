#pragma once
#include <atomic>
#include <array>

#include <EXPERIMENTAL/EXPERIMENTAL_RUNTIME_DATA_STRUCTURES.h>
#include <ManifestUtility/Typenames.h>
#include <ManifestUtility/DebugLogger.h>

namespace Manifest_Memory
{	
	using Generation = MFu32;

	template<typename T, typename Deleter>
	class RCU
	{
	private:		
		static constexpr Generation MAX_RCU_GENERATION{ 2 };
		static constexpr MFu32 RCU_MODULO{ MAX_RCU_GENERATION - 1 };
		struct ReadFlag
		{
			std::atomic<MFbool> isReading{ false };
			char padding[64 - sizeof(isReading)];
		};
		struct GenerationHandle
		{
			Generation generation;
			T* handle;
		};
		//rcu				
		const Deleter deleter;
		std::atomic<Generation> globalGeneration;
		GenerationHandle generationHandles[MAX_RCU_GENERATION];
		std::array<ReadFlag*, MAX_RCU_GENERATION> generationReadFlags;		
		//registration
		const MFsize maxReaders;
		std::atomic<MFu32> registeredReaders;
	public:
		using Handle = GenerationHandle;
		RCU() = default;
		RCU(const MFsize _maxReaders)
			: maxReaders(_maxReaders), registeredReaders{ 0 }, deleter{ Deleter{} }, globalGeneration{ 0 }
		{
			for (Generation generation{ 0 }; generation < MAX_RCU_GENERATION; ++generation)
			{
				generationHandles[generation] = GenerationHandle{ 0,new T };
				auto& readFlag = generationReadFlags[generation];
				readFlag = new ReadFlag[maxReaders];				memset(readFlag, 0, sizeof(ReadFlag) * maxReaders);
			}
		}		
		GenerationHandle rcu_read_lock(const MFu32& readerId)
		{
			//use current generation as guess
			Generation currentGeneration = globalGeneration.load(std::memory_order_relaxed);
			MFu32 generationIndex = currentGeneration & RCU_MODULO;
			//block writer from deleting from index
			generationReadFlags[generationIndex][readerId].isReading.store(true, std::memory_order_release);
			Generation oldGeneration = currentGeneration;
			//if guess was invalid - remove block and try again
			while (oldGeneration != (currentGeneration = globalGeneration.load(std::memory_order_acquire)))
			{
				DLOG(33, "inconsistent state detected!");
				MFu32 oldIndex = generationIndex;
				generationIndex = currentGeneration & RCU_MODULO;
				generationReadFlags[oldIndex][readerId].isReading.store(false, std::memory_order_relaxed);
				generationReadFlags[generationIndex][readerId].isReading.store(true, std::memory_order_release);
				oldGeneration = currentGeneration;
			}
			//return a handle to the read-locked generation 
			return generationHandles[generationIndex];
		}
		void rcu_read_unlock(const GenerationHandle& generationHandle, const MFu32& readerId)
		{
			//unblock read generation for writer
			MFu32 generationIndex = generationHandle.generation & RCU_MODULO;
			generationReadFlags[generationIndex][readerId].isReading.store(false, std::memory_order_relaxed);
		}
		void synchronize_rcu(T* update)
		{
			//store old generation for current readers;
			Generation oldGeneration = globalGeneration.load(std::memory_order_relaxed);
			MFu32 oldIndex = oldGeneration & RCU_MODULO;;
			//prepare new geneartion for future readers
			Generation newGeneration = oldGeneration + 1;
			Handle newHandle{ newGeneration,update };
			MFu32 newIndex = newGeneration & RCU_MODULO;
			//make new generation visible for future readers
			generationHandles[newIndex] = newHandle;
			globalGeneration.store(newGeneration, std::memory_order_release);
			//wait for old readers
			for (auto reader{ 0 }; reader < registeredReaders.load(std::memory_order_relaxed); ++reader)
				while (generationReadFlags[oldIndex][reader].isReading.load(std::memory_order_acquire))DLOG(32,"Waiting on reader:" << reader);
			//release unused memory
			deleter(generationHandles[oldIndex].handle);
			generationHandles[oldIndex].handle = nullptr;
		}
		MFu32 RegisterReader()
		{
			assert(registeredReaders.load(std::memory_order_seq_cst) < maxReaders);
			return registeredReaders.fetch_add(1, std::memory_order_relaxed);
		}
	};
}