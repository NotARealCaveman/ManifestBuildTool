#pragma once
#include <functional>
#include <type_traits>

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

		template<typename Function, typename... Params>
		decltype(auto) TableForwardingFunction(Function&& function, Params&&... params)
		{
			return std::forward<Function>(function)(std::forward<Params>(params)...);
		};		

		RCU rcu;	
		ExchangeLock writeLock;
	public:			
		Table(const MFsize maxConcurrentReaders)
			: rcu{ maxConcurrentReaders } {};		
		//params are taken by reference - be careful to not make unwated alterations to parameter data
		template<typename Function, typename... Params>
		inline void Push(Function&& function, Params&&... params)
		{			
			writeLock.Lock();
			rcu.synchronize_rcu(TableForwardingFunction(function, params...));
			writeLock.Unlock();
		};
		
		template<typename Function, typename... Params>
		using TableReturnType = decltype(std::declval<Function>()(std::declval<typename RCU::Handle>(), std::declval<Params>()...));
			
		//return type pull - for types that can be internally constructed 
		template<typename Function, typename... Params>
		typename std::enable_if<!std::is_same<TableReturnType<Function, Params...>, void>::value, TableReturnType<Function, Params...>>::type Pull(const MFu32 readerId, Function&& function, Params&&... params)
		{
			typename RCU::Handle handle = rcu.rcu_read_lock(readerId);
			auto&& result{ TableForwardingFunction(function, handle, params...) };
			rcu.rcu_read_unlock(handle, readerId);

			return std::move(result);
		};

		//void specialized pull - for types that need external constructions
		template<typename Function, typename... Params>
		typename std::enable_if<std::is_same<TableReturnType<Function,Params...>,void>::value,void>::type Pull(const MFu32 readerId, Function&& function, Params&&... params)
		{
			typename RCU::Handle handle = rcu.rcu_read_lock(readerId);
			TableForwardingFunction(function, handle, params...);
			rcu.rcu_read_unlock(handle, readerId);
		};
		//if successful returns an index reserved for reader
		inline MFu32 ReserveTableReadFlag()
		{
			return rcu.RegisterReader();
		}
	};
}
