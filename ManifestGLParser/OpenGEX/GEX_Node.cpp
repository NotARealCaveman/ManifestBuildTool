#include "GEX_Node.h"

using namespace Manifest_Parser;

DDL_Structure* GEX_Node::Build(const std::string& subStructure, DDL_ReferenceMap& referenceMap)
{
	auto result = New<DDL_Structure, ScratchPad<DDL_Structure>>(1);
	ParseStructureHeader(subStructure, *result);
	
	switch (ExtractStructureType(subStructure))
	{
		case GEX_BufferTypes::GEX_Name:
		{		
			result = name.Build(subStructure, referenceMap);			
			result->typeHeap = static_cast<void*>(&name);
			break;
		}
		case GEX_BufferTypes::GEX_Transform:
		{			
			result = transforms.emplace_back().Build(subStructure, referenceMap);			
			result->typeHeap = static_cast<void*>(&transforms.back());
			break;
		}
		DEFAULT_BREAK
	}	
	MapStructureName(*result, referenceMap);

	return result;
}

void Manifest_Parser::ReserveNodeContainers(GEX_Node& node)
{
	node.transforms.reserve(VECTOR_RESERVATION_SIZE);
	node.animations.reserve(VECTOR_RESERVATION_SIZE);
	node.boneNode.reserve(VECTOR_RESERVATION_SIZE);
	node.geometryNode.reserve(VECTOR_RESERVATION_SIZE);
	node.cameraNode.reserve(VECTOR_RESERVATION_SIZE);
	node.lightNode.reserve(VECTOR_RESERVATION_SIZE);
}