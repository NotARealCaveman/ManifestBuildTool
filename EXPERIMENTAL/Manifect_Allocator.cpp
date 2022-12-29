#include "Manifest_Allocator.h"

using namespace Manifest_Memory;

void Manifest_Memory::RegisterProgramExecutiveThread()
{
	auto id = std::this_thread::get_id();
	while (registerLock.exchange(1, std::memory_order_acquire));
	REGISTERED_PROGRAM_EXECUTIVE_THREAD_IDS[registeredExecutiveThreads] = id;
	++registeredExecutiveThreads;
	registerLock.store(0, std::memory_order_release);
}

ThreadMemoryHandles* Manifest_Memory::GetThreadMemoryHandles()
{
	const auto id = std::this_thread::get_id();
	for (auto threadID{ 0 }; threadID < NUMBER_PROGRAM_EXECUTIVE_THREADS; ++threadID)
		if (id == REGISTERED_PROGRAM_EXECUTIVE_THREAD_IDS[threadID])
			return &PROGRAM_EXECUTIVE_THREAD_MEMORY_HANDLES[threadID];
	return nullptr;
}

void Manifest_Memory::INIT_MEMORY_RESERVES()
{
	auto& memoryHandles = PROGRAM_EXECUTIVE_THREAD_MEMORY_HANDLES[0];
	memoryHandles.linearBegin = memoryHandles.linearHeap = memoryHandles.threadHeap = static_cast<Byte*>(malloc(1024));

};