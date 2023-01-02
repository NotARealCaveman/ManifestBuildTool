#include "DDL_Primitives.h"

using namespace Manifest_Parser;

DDL_Structure DDL_String::Build(const ScratchPadString& partitionedStructure, DDL_ReferenceMap& referenceMap)
{	
	DDL_Structure result;
	ParseStructureHeader(partitionedStructure, result);
	//check for sub buffer data
	auto subBufferIdentifier = partitionedStructure.find_first_of('[');
	auto payloadIndex = partitionedStructure.find_first_of('"');
	if (subBufferIdentifier == std::string::npos)
	{		
		auto payload = partitionedStructure.substr(payloadIndex+1);
		data = payload.substr(0, payload.find_first_of('"'));		
	}	
	result.typeHeap = static_cast<void*>(&data);
	MapStructureName(result, referenceMap);

	return result;
}