#pragma once

#include <ManifestMemory/MemoryGuards/RCU.h>
#include <ManifestMemory/MemoryGuards/ExchangeLock.h>
#include <ManifestPhysics/State.h>


#include "Runtime_TableIterator.h"

using namespace Manifest_Utility;
using namespace Manifest_Memory;

namespace Manifest_Persistence
{
	template<typename T,typename Deleter>
	class Table
	{
	private:
		using RCU = RCU<T, Deleter>;
		template<typename... Params>
		using WriteFunction = T * (*)(Params&...);		
		using ReadFunction = void(*)(const typename RCU::Handle&, T*);

		RCU rcu;	
		ExchangeLock writeLock;
	public:			
		Table(const MFsize maxConcurrentReaders)
			: rcu{ maxConcurrentReaders } {};
		Table(Table&& other)
			: rcu{ std::move(other.rcu) } {};
		//params are taken by reference - be careful to not make unwated alterations to parameter data
		template<typename... Params>	
		inline void Push(WriteFunction<Params...> writeFunction, Params&... params)
		{			
			writeLock.Lock();
			rcu.synchronize_rcu(writeFunction(params...));
			writeLock.Unlock();
		};
				
		void Pull(const MFu32& readerId, ReadFunction readFunction, T* copyReadToAddress)
		{
			typename RCU::Handle handle = rcu.rcu_read_lock(readerId);
			readFunction(handle, copyReadToAddress);
			rcu.rcu_read_unlock(handle, readerId);
		};
		//if successful returns an index reserved for reader
		inline MFu32 ReserveTableReadFlag()
		{
			return rcu.RegisterReader();
		}
	};
}
