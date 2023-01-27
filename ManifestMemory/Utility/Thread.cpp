#include "Thread.h"

using namespace Manifest_Memory;

ThreadJoiner::ThreadJoiner(std::vector<std::thread>& _threads) :threads{ _threads }
{}

ThreadJoiner::~ThreadJoiner()
{
	for (unsigned long i = 0; i < threads.size(); ++i)
	{
		if (threads[i].joinable())
			threads[i].join();
	}
}