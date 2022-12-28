#include "Manifest_Allocator.h"

using namespace Manifest_Memory;

Byte* ProgramMemory::threadMainHeaps[NUMBER_OF_EXECUTIVE_THREADS] {nullptr};
Freelist* ProgramMemory::threadFreelists[NUMBER_OF_EXECUTIVE_THREADS]{ nullptr };
std::vector<std::thread::id> ProgramMemory::allocationThreads{};

Byte* ProgramMemory::linearBegin[NUMBER_OF_EXECUTIVE_THREADS]{ nullptr };
Byte*  ProgramMemory::linearHeap[NUMBER_OF_EXECUTIVE_THREADS]{ nullptr };
MFsize ProgramMemory::usedBytes[NUMBER_OF_EXECUTIVE_THREADS]{ 0 };
MFu64 ProgramMemory::nAllocations[NUMBER_OF_EXECUTIVE_THREADS]{ 0 };

std::vector<std::thread::id>::const_iterator ProgramMemory::ThisThread(const std::vector<std::thread::id>::const_iterator& begin, const std::vector<std::thread::id>::const_iterator& end)
{
	return std::find(begin, end, std::this_thread::get_id());
}

Freelist* ProgramMemory::GetThreadFreelist(MFu32* threadIndex)
{	
	const auto begin = ProgramMemory::allocationThreads.begin();
	const auto end = ProgramMemory::allocationThreads.end();
	auto thread = ThisThread(begin,end);
	*threadIndex = thread - begin;

	return thread != end ? ProgramMemory::threadFreelists[*threadIndex] : nullptr;
}

Byte* ProgramMemory::GetThreadLinearHeap(MFu32* threadIndex)
{
	const auto begin = ProgramMemory::allocationThreads.begin();
	const auto end = ProgramMemory::allocationThreads.end();
	auto thread = ThisThread(begin, end);
	*threadIndex = thread - begin;

	return thread != end ? ProgramMemory::linearHeap[*threadIndex] : nullptr;
}

void Manifest_Memory::INIT_MEMORY_RESERVES()
{
	//t1	
	ProgramMemory::linearBegin[0] = ProgramMemory::linearHeap[0] = ProgramMemory::threadMainHeaps[0] = static_cast<Byte*>(malloc(1024));
}