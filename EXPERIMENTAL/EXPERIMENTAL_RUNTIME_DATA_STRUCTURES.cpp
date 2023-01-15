#include "EXPERIMENTAL_RUNTIME_DATA_STRUCTURES.h"

using namespace Manifest_Experimental;

//exchange spin lock - spin locally
void ExchangeLock::Lock()
{
	while (true)
	{//if currently unlocked !false->true, else remains locked
		if (!lock.exchange(LOCKED, std::memory_order_acquire))
			return;
	//just read lock value without write
		while (lock.load(std::memory_order_relaxed));
	}
};

void ExchangeLock::Unlock()
{
	lock.store(UNLOCKED, std::memory_order_release);
};

//RW lock
const bool SRSWExchangeLock::Try_Write()
{
	return reading.test(std::memory_order_relaxed);
}
const bool SRSWExchangeLock::Try_Read()
{
	return writing.test(std::memory_order_relaxed);
}
