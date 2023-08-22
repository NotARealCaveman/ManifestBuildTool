#include "DDL_Reference.h"

using namespace Manifest_Parser;

DDL_Structure* DDL_Reference::Build(const std::string_view& partitionedStructureView, DDL_ReferenceMap& referenceMap)
{
	auto result = New<DDL_Structure, ScratchPad<DDL_Structure>>(1);
	ParseStructureHeader(partitionedStructureView, *result);
	referenceNames = PartitionStructureReferences(partitionedStructureView);	
	result->typeHeap = static_cast<void*>(this);
	MapStructureName(*result, referenceMap);
	DLOG({ CONSOLE_COLOR::BLUE }, "typeheap: " , result->typeHeap);

	return result;
}

void Manifest_Parser::ReserveReferenceContainers(DDL_Reference& reference)
{
	reference.referenceNames.reserve(VECTOR_RESERVATION_SIZE);
}