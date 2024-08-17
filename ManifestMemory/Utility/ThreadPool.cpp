#include "ThreadPool.h"

using namespace Manifest_Memory;

ThreadPool::ThreadPool()
	:finished{ false }, joiner(threads)
{
	//1 thread for main, 1 thread for renderer
	constexpr MFu32 maxThreads{ 2 };//don't melt test PCs
	const MFu32 nThreads{ std::thread::hardware_concurrency() > maxThreads ? maxThreads : std::thread::hardware_concurrency() };
	for (uint32_t i = 0; i < nThreads; ++i)
		threads.push_back(std::thread(&ThreadPool::Worker, this));
}

void ThreadPool::Worker()
{
	while (!finished)
	{		
		FunctionWrapper_OLD task;
		if (queue.TryPop(task))
			task();
		else
			std::this_thread::sleep_for(std::chrono::duration<float, std::milli>{20});
	}
}

ThreadPool::~ThreadPool()
{
	finished = true;
}//