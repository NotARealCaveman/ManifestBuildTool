#pragma once
#include <atomic>
#include <array>

#include "Typenames.h"
#include <ManifestGLUtility/DebugLogger.h>
#include <EXPERIMENTAL/EXPERIMENTAL_RUNTIME_DATA_STRUCTURES.h>

namespace Manifest_Experimental
{
	using Generation = MFu32;
	constexpr Generation MAX_RCU_GENERATION{ 4 };
	constexpr Generation FREE_GENERATION{ UINT32_MAX };
		
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
		};
		//cannot move or copy RCU
		RCU(const RCU&) = delete;
		RCU(const RCU&&) = delete;
		RCU() : deleter{ Deleter{} } {};
		//Reader lock
		Generation rcu_read_lock()
		{
			//use current generation as guess
			Generation currentGeneration = globalGeneration.load(std::memory_order_acquire);
			//find the slot the generation would be written to 
			auto generationIndex = currentGeneration % MAX_RCU_GENERATION;
			//add read attempt to the slot
			dataGenerations[generationIndex].readers.fetch_add(1, std::memory_order_acq_rel);
			//validate read attempt was good
			while (currentGeneration != globalGeneration.load(std::memory_order_acquire))
			{
				//remove bad read attempt
				dataGenerations[generationIndex].readers.fetch_sub(1, std::memory_order_acq_rel);
				//get new generation guess
				currentGeneration = globalGeneration.load(std::memory_order_acquire);
				generationIndex = currentGeneration % MAX_RCU_GENERATION;
				//make new read attempt
				dataGenerations[generationIndex].readers.fetch_add(1, std::memory_order_acq_rel);
			}
			//return good read generation - guaranteed this generation will remain alive for reader
			return currentGeneration;
		};
		//reader unlock
		void rcu_read_unlock(const Generation& readGeneration)
		{
			//remove read from preserved generation
			auto generationIndex = readGeneration % MAX_RCU_GENERATION;
			//unblock writer attempting to clean up generation
			dataGenerations[generationIndex].readers.fetch_sub(1, std::memory_order_acq_rel);
		};
		//writer update 
		void synchronize_rcu(T* update)
		{
			//find next slot for writing
			Generation nextGeneration = globalGeneration.load(std::memory_order_relaxed) + 1;
			auto generationIndex = nextGeneration % MAX_RCU_GENERATION;
			//write data for future readers
			dataGenerations[generationIndex].data = update;
			//atomically update generation, storing the previous
			Generation previousGeneration = globalGeneration.fetch_add(1, std::memory_order_acq_rel);
			generationIndex = previousGeneration % MAX_RCU_GENERATION;
			//wait on readers to leave		
			while (dataGenerations[generationIndex].readers.load(std::memory_order_acquire));
			/*int waited{0};
			while (dataGenerations[generationIndex].readers.load(std::memory_order_acquire))
			{
				++waited;
			};
			if (waited)
				LOG(33, "waited: " << waited);*/
			
			//release previous data
			deleter(dataGenerations[generationIndex].data);
		};

		const Deleter deleter;
		std::array<DataGeneration<T>, MAX_RCU_GENERATION> dataGenerations;		
		std::atomic<Generation> globalGeneration;		
	};
}