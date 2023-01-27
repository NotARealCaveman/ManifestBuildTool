#include "ThreadPool.h"

using namespace Manifest_Memory;

ThreadPool::ThreadPool()
	:finished{ false }, joiner(threads)
{
	//1 thread for main, 1 thread for renderer
	numberWorkerThreads= std::thread::hardware_concurrency()-2;
	for (uint32_t i = 0; i < numberWorkerThreads; ++i)
		threads.push_back(std::thread(&ThreadPool::Worker, this));
}

void ThreadPool::Worker()
{
	while (!finished)
	{		
		FunctionWrapper task;
		if (queue.TryPop(task))
			task();
		else
			std::this_thread::yield();
	}
}

ThreadPool::~ThreadPool()
{
	finished = true;
}//