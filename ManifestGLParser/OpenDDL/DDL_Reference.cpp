#include "DDL_Reference.h"

using namespace Manifest_Parser;

DDL_Structure DDL_Reference::Build(const std::string& partitionedStructure, DDL_ReferenceMap& referenceMap)
{
	DDL_Structure result;
	ParseStructureHeader(partitionedStructure, result);
	referenceNames = PartitionStructureReferences(partitionedStructure);	
	result.typeHeap = static_cast<void*>(&referenceNames);
	MapStructureName(result, referenceMap);

	return result;
}

void Manifest_Parser::ReserveReferenceContainers(DDL_Reference& reference)
{
	reference.referenceNames.reserve(VECTOR_RESERVATION_SIZE);
}