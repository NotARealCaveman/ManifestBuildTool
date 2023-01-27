#pragma once
#include <atomic>

#include <ManifestUtility/Typenames.h>

using namespace Manifest_Utility;

namespace Manifest_Memory
{
	//loop on exchange while lock is held(1) until released(0)
	constexpr MFbool LOCKED{ 1 };
	constexpr MFbool UNLOCKED{ 0 };
	struct ExchangeLock
	{
	protected:
		std::atomic<MFbool> lock{ UNLOCKED };
	public:
		//attempts to lock, spins on exchange until locked
		void Lock();
		//restores lock to unlocked state
		void Unlock();
	};
}