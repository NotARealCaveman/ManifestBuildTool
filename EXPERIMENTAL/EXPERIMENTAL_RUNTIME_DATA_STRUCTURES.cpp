#include "EXPERIMENTAL_RUNTIME_DATA_STRUCTURES.h"

using namespace Manifest_Experimental;

//exchange spin lock - spin locally & back off
void ExchangeLock::Lock()
{
	while (true)
	{//if currently unlocked !false->true, else remains locked
		if (!lock.exchange(LOCKED, std::memory_order_acquire))
			return;
	//just read lock value without write
		while (lock.load(std::memory_order_relaxed))
			for (volatile int i{ 0 }; i < 50; ++i);
	}
};

void ExchangeLock::Unlock()
{
	lock.store(UNLOCKED, std::memory_order_release);
};
