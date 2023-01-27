#pragma once
#include <iostream>
#include <thread>
#include <functional>
#include <vector>
#include <mutex>

#include <ManifestUtility/DebugLogger.h>


namespace Manifest_Memory
{
	class ThreadJoiner
	{
	private:
		std::vector<std::thread>& threads;
	public:
		explicit ThreadJoiner(std::vector<std::thread>& threads_);
		~ThreadJoiner();
	};
}
