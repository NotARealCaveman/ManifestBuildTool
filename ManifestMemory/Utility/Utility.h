#pragma once
#include <vector>
#include <future>
#include <thread>
#include <algorithm>
#include <atomic>

#include <ManifestGLUtility/DebugLogger.h>

#include "Iterator.h"
#include "Thread.h"

namespace Manifest_Memory
{
	template<typename Itr, typename Function>
	void ParallelForEach(Itr first, Itr last, Function f)
	{
		const uint32_t length = distance(first, last);
		if (!length)
			return;

		const uint32_t minPerThread = 25;
		const uint32_t maxThreads = (length + minPerThread - 1) / minPerThread;
		const uint32_t availableThreads = std::thread::hardware_concurrency();
		const uint32_t numThreads = std::min(availableThreads != 0 ? availableThreads : 2, maxThreads);
		const uint32_t blockSize = length / numThreads;
		
		std::vector<std::future<void>> futures(numThreads - 1);
		std::vector<std::thread> threads(numThreads - 1);
		ThreadJoiner joiner(threads);
		Itr blockStart = first;
		for (uint32_t i = 0; i < numThreads - 1; ++i)
		{
			Itr blockEnd = blockStart;
			advance(blockEnd, blockSize);
			std::packaged_task<void(void)> task([=]()
			{
				std::for_each(blockStart, blockEnd, f);
			});
			futures[i] = task.get_future();
			threads[i] = std::thread(std::move(task));
			blockStart = blockEnd;
		}
		std::for_each(blockStart, last, f);		
	}

	template<typename Itr, typename SearchType>
	Itr ParalellFind(Itr first, Itr last, SearchType search)
	{
		struct Find
		{
			void operator()(Itr begin, Itr end, SearchType match, std::promise<Itr>& result,std::atomic<bool>& found)
			{
				for (; (begin != end) && !found.load(); ++begin)
					if (*begin == match)
					{
						result.set_value(begin);
						found.store(true);
						return;
					}
			}
		};
		const uint32_t length = distance(first, last);
		const uint32_t minPerThread = 1;
		const uint32_t maxThreads = (length + minPerThread - 1) / minPerThread;
		const uint32_t availableThreads = std::thread::hardware_concurrency();
		const uint32_t numThreads = std::min(availableThreads != 0 ? availableThreads : 2, maxThreads);
		const uint32_t blockSize = length / numThreads;
		std::promise<Itr> result;
		std::atomic<bool> found(false);
		std::vector<std::thread> threads(numThreads - 1);
		//join threads on quick exit
		{
			ThreadJoiner joiner(threads);
			Itr blockStart = first;
			for (uint32_t i = 0; i < numThreads - 1; ++i)
			{
				Itr blockEnd = blockStart;
				advance(blockEnd, blockSize);
				threads[i] = std::thread(Find(), blockStart, blockEnd, search, std::ref(result), std::ref(found));
				blockStart = blockEnd;
			}
			Find()(blockStart, last, search, std::ref(result), std::ref(found));
		}
		//if not found return 1PTE-Itr
		if (!found.load())
			return last;
		return result.get_future().get();
	}

	template<typename Itr, typename Predicate>
	Itr ParalellFindIf(Itr first, Itr last, Predicate p)
	{
		struct Find
		{
			void operator()(Itr begin, Itr end, Predicate p, std::promise<Itr>& result, std::atomic<bool>& found)
			{
				for (; (begin != end) /* && !found.load() */ ; ++begin)
				{
					if (p(*begin))
					{
						result.set_value(begin);
						found.store(true);
						return;
					}
				}
			}
		};
		const uint32_t length = distance(first, last);
		const uint32_t minPerThread = 1;
		const uint32_t maxThreads = (length + minPerThread - 1) / minPerThread;
		const uint32_t availableThreads = std::thread::hardware_concurrency();
		const uint32_t numThreads = std::min(availableThreads != 0 ? availableThreads : 2, maxThreads);
		const uint32_t blockSize = length / numThreads;
		std::promise<Itr> result;
		std::atomic<bool> found(false);
		std::vector<std::thread> threads(numThreads - 1);
		//join threads on quick exit
		{
			ThreadJoiner joiner(threads);
			Itr blockStart = first;
			for (uint32_t i = 0; i < numThreads - 1; ++i)
			{
				Itr blockEnd = blockStart;
				advance(blockEnd, blockSize);
				threads[i] = std::thread(Find(), blockStart, blockEnd, p, std::ref(result), std::ref(found));
				blockStart = blockEnd;
			}
			Find()(blockStart, last, p, std::ref(result), std::ref(found));
		}
		//if not found return 1PTE-Itr
		if (!found.load())
			return last;
		return result.get_future().get();
	}

	inline void Swap(float& a, float& b)
	{
		auto t = a;
		a = b;
		b = t;
	}
	inline void Swap(double& a, double& b)
	{
		auto t = a;
		a = b;
		b = t;
	}
}