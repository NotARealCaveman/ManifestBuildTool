#include "DDL_Primitives.h"

using namespace Manifest_Parser;

DDL_Structure* DDL_String::Build(const std::string_view& partitionedStructureView, DDL_ReferenceMap& referenceMap)
{	
	auto result = New<DDL_Structure, ScratchPad<DDL_Structure>>(1);
	ParseStructureHeader(partitionedStructureView, *result);
	//check for sub buffer data
	auto subBufferIdentifier = partitionedStructureView.find_first_of('[');
	auto payloadIndex = partitionedStructureView.find_first_of('"');
	if (subBufferIdentifier == std::string::npos)
	{		
		auto payload = partitionedStructureView.substr(payloadIndex+1);
		data = payload.substr(0, payload.find_first_of('"'));		
	}	
	result->typeHeap = static_cast<void*>(&data);
	MapStructureName(*result, referenceMap);

	return result;
}