#pragma once

#include <ManifestMemory/MemoryGuards/RCU.h>
#include <ManifestMemory/MemoryGuards/ExchangeLock.h>


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
		template<typename... Params>
		using ReadFunction = void(*)(const typename RCU::Handle&, Params&...);
		template<typename... Params>
		using ReadCopyFunction = void(*)(const typename RCU::Handle&, T*, Params&...);

		RCU rcu;	
		ExchangeLock writeLock;
	public:			
		Table(const MFsize maxConcurrentReaders)
			: rcu{ maxConcurrentReaders } {};		
		//params are taken by reference - be careful to not make unwated alterations to parameter data
		template<typename... Params>	
		inline void Push(WriteFunction<Params...> writeFunction, Params&... params)
		{			
			writeLock.Lock();
			rcu.synchronize_rcu(writeFunction(params...));
			writeLock.Unlock();
		};	
		template<typename... Params>
		void Pull(const MFu32& readerId, ReadFunction<Params...> readFunction, Params&... params)
		{
			typename RCU::Handle handle = rcu.rcu_read_lock(readerId);
			readFunction(handle,params...);
			rcu.rcu_read_unlock(handle, readerId);
		};
		template<typename... Params>
		void Pull(const MFu32& readerId, ReadCopyFunction<Params...> readCopyFunction, T* readDataTransform, Params&... params)
		{
			typename RCU::Handle handle = rcu.rcu_read_lock(readerId);
			readCopyFunction(handle, readDataTransform, params...);
			rcu.rcu_read_unlock(handle, readerId);
		};
		//if successful returns an index reserved for reader
		inline MFu32 ReserveTableReadFlag()
		{
			return rcu.RegisterReader();
		}
	};
}
