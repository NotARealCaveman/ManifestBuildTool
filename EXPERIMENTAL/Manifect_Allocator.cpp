#include "Manifest_Allocator.h"

using namespace Manifest_Memory;

Byte* ProgramMemory::threadHeaps[NUMBER_OF_EXECUTIVE_THREADS] {nullptr};
Freelist* ProgramMemory::threadFreelists[NUMBER_OF_EXECUTIVE_THREADS]{ nullptr };
std::vector<std::thread::id> ProgramMemory::allocationThreads{};

Freelist* ProgramMemory::GetThreadFreelist()
{	
	const auto callingThread = std::this_thread::get_id();	
	const auto& threads = ProgramMemory::allocationThreads;
	const auto thread = std::find(threads.begin(), threads.end(), callingThread);
	return thread != threads.end() ? ProgramMemory::threadFreelists[thread - threads.begin()] : nullptr;	
}