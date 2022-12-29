#include "Manifest_Allocator.h"

using namespace Manifest_Memory;

void Manifest_Memory::RegisterProgramExecutiveThread()
{
	auto id = std::this_thread::get_id();
	while (registerLock.exchange(1, std::memory_order_acquire));
	REGISTERED_PROGRAM_EXECUTIVE_THREAD_IDS[registeredExecutiveThreads++] = id;
	registerLock.store(0, std::memory_order_release);
}

void Manifest_Memory::INIT_MEMORY_RESERVES()
{};