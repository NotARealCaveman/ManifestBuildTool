#include "EXPERIMENTAL_RUNTIME_DATA_STRUCTURES.h"

using namespace Manifest_Experimental;

//exchange spin lock
void ExchangeLock::Lock()
{
	int waitCount{ 0 };
	while (lock.exchange(LOCKED, std::memory_order_acquire))++waitCount;
	if (waitCount)
		DLOG(36, std::this_thread::get_id() << " waiting on lock(" << waitCount << ")");
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
