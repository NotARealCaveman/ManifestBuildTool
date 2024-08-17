#pragma once
#include <atomic>
#include <future>
#include <type_traits>

#include <ManifestUtility/Typenames.h>

#include <ManifestMemory/Containers/Queue.h>
#include "Thread.h"
#include "FunctionWrapper.h"

using namespace Manifest_Memory;
using namespace Manifest_Utility;

namespace Manifest_Memory
{
	class ThreadPool
	{
	private:
		void Worker();		
		std::vector<std::thread> threads;
		std::atomic<MFbool> finished;		
		ThreadJoiner joiner;		
		ThreadedQueue<FunctionWrapper_OLD> queue;	
	public:
		ThreadPool();
		template<typename R,typename Function, typename... Args>
		std::future<R> SubmitTask(Function function)
		{					
			std::packaged_task<R(Args...)> task(std::move(function));
			std::future<R> res(task.get_future());
			queue.Push(std::move(task));
			return res;
		} 
		~ThreadPool();

		MFu32 numberWorkerThreads;
	};
}