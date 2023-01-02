#include "GEX_GeometryObject.h"

using namespace Manifest_Parser;

const std::map<std::string, DDL_BufferType> GEX_GeometryNode::PropertyList::typeProperties
{
	{ "visible", PropertyList::VISIBLE},
	{ "shadow", PropertyList::SHADOW},
	{ "motion_blur", PropertyList::MOTIONBLUR},	
};

DDL_Structure GEX_GeometryNode::Build(const ScratchPadString& partitionedStructure, DDL_ReferenceMap& referenceMap)
{
	ReserveNodeContainers(*this);
	materialRefs.reserve(VECTOR_RESERVATION_SIZE);
	morphWeights.reserve(VECTOR_RESERVATION_SIZE);

	DDL_Structure result;
	for (const DDL_Property& property : PartitionStructureProperties(ParseStructureHeader(partitionedStructure, result)))
		switch (PropertyList::typeProperties.find(property.key)->second)
		{
			case PropertyList::VISIBLE:
				std::stringstream{ property.value } >> std::boolalpha >> *(visible = New<MFbool,ScratchPad<MFbool>>(1));
				break;
			case PropertyList::SHADOW:
				std::stringstream{ property.value } >> std::boolalpha >> *(shadow = New<MFbool, ScratchPad<MFbool>>(1));
				break;
			case PropertyList::MOTIONBLUR:
				std::stringstream{ property.value } >> std::boolalpha >> *(motion_blur = New<MFbool, ScratchPad<MFbool>>(1));
				break;
			DEFAULT_BREAK
		}

	//build substructures
	for (const ScratchPadString& subStructure : PartitionDDLSubStructures(partitionedStructure))
		switch (ExtractStructureType(subStructure))
		{			
			case GEX_BufferTypes::GEX_ObjectRef:			
				result.subSutructres.emplace_back(objectRef.Build(subStructure, referenceMap));
				objectRef.type = GEX_ObjectType::GeometryObject;
				break;			
			case GEX_BufferTypes::GEX_MaterialRef:			
				result.subSutructres.emplace_back(materialRef.Build(subStructure, referenceMap));
				break;			
			case GEX_BufferTypes::GEX_MorphWeight:			
				break;			
			default:			
				result.subSutructres.emplace_back(GEX_Node::Build(subStructure, referenceMap));
				break;			
		}			
	result.typeHeap = static_cast<void*>(this);
	MapStructureName(result, referenceMap);

	return result;
}

const std::map<std::string, DDL_BufferType> GEX_GeometryObject::PropertyList::typeProperties
{
	{ "visible", PropertyList::VISIBLE},
	{"shadow", PropertyList::SHADOW},
	{"motion_blur", PropertyList::MOTIONBLUR},
};

DDL_Structure GEX_GeometryObject::Build(const ScratchPadString& partitionedStructure, DDL_ReferenceMap& referenceMap)
{
	DDL_Structure result;
	
	for (const DDL_Property& property : PartitionStructureProperties(ParseStructureHeader(partitionedStructure, result)))
		switch (PropertyList::typeProperties.find(property.key)->second)
		{			
			case PropertyList::VISIBLE:			
				std::stringstream{ property.value } >> std::boolalpha >> visible;
				break;			
			case PropertyList::SHADOW:
				std::stringstream{ property.value } >> std::boolalpha >> shadow;
				break;			
			case PropertyList::MOTIONBLUR:
				std::stringstream{ property.value } >> std::boolalpha >> motion_blur;
				break;			
			DEFAULT_BREAK
		}
	for (const auto& subStructure : PartitionDDLSubStructures(partitionedStructure))
		switch (ExtractStructureType(subStructure))
		{
			case GEX_BufferTypes::GEX_Mesh:
				//result.subSutructres.emplace_back(mesh.Build(subStructure, referenceMap));
				break;
			case GEX_BufferTypes::GEX_Morph:
				result.subSutructres.emplace_back(morph.Build(subStructure, referenceMap));
				break;
			DEFAULT_BREAK
		}	
	result.typeHeap = static_cast<void*>(this);
	MapStructureName(result, referenceMap);

	return result;
}
