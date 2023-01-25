#pragma once

#include <EXPERIMENTAL/RCU.h>

#include "Runtime_TableIterator.h"

using namespace Manifest_Experimental;

namespace Manifest_Persistence
{
	template<typename T, typename Deleter>
	class Table
	{
	private:
		using RCU = RCU<T, Deleter>;

		RCU rcu;
	public:		
		Table(const MFsize maxConcurrentReaders)
			:rcu{ maxConcurrentReaders } {};	
		//if successful returns an index reserved for reader
		inline MFu32 ReserveTableReadFlag()
		{
			return rcu.RegisterReader();
		}		
		inline void Push(T* commit)
		{
			rcu.synchronize_rcu(commit);
		};
		template<typename ReadFunction, typename... Params>
		void Pull(const MFu32 readerId, const ReadFunction& readFunction, Params... params)
		{
			typename RCU::Handle handle = rcu.rcu_read_lock(readerId);
			readFunction(params...);
			rcu.rcu_read_unlock(handle, readerId);
		};
	};
}
