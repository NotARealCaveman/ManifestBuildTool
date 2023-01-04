#include "GEX_Mesh.h"

using namespace Manifest_Parser;

const std::map<const char*, DDL_BufferType> GEX_VertexArray::PropertyList::typeProperties
{
	{ "attrib", PropertyList::ATTRIB},
	{ "index",PropertyList::INDEX},
	{ "morph",PropertyList::MORPH},
};

DDL_Structure* GEX_VertexArray::Build(const std::string& partitionedStructure, DDL_ReferenceMap& referenceMap)
{	
	auto result = New<DDL_Structure, ScratchPad<DDL_Structure>>(1);
	for (const DDL_Property& property : PartitionStructureProperties(ParseStructureHeader(partitionedStructure, *result)))
		switch (PropertyList::typeProperties.find(property.key.c_str())->second)
		{
		case PropertyList::ATTRIB:
			attrib = property.value;
			bufferIndex = GetBufferIndex(attrib.c_str());
			break;
		case PropertyList::INDEX:
			index = std::stoi(property.value.c_str());
			break;
		case PropertyList::MORPH:
			morph = std::stoi(property.value.c_str());
			break;
		DEFAULT_BREAK
		}	
	result->subSutructres.emplace_back(vertexArray.Build(PartitionDDLSubStructuresV2({ partitionedStructure.c_str() })[0].c_str(), referenceMap));//should only have 1
	result->typeHeap = static_cast<void*>(this);
	MapStructureName(*result, referenceMap);

	return result;
}

uint8_t Manifest_Parser::GetBufferIndex(const ScratchPadString& bufferAttrib)
{	
	auto temp = bufferAttrib;
	temp.erase(std::remove(temp.begin(), temp.end(), '"'),temp.end());
	std::transform(temp.begin(), temp.end(),temp.begin(), [&](const unsigned char& letter) {return std::tolower(letter); });	
	if (temp == "position")
		return 0;
	else if(temp == "texcoord")
		return 1;
	else if (temp == "normal")
		return 2;
	else if (temp == "tangent")
		return 3;
	else if (temp == "bitangent")
		return 4;
}

const std::map<std::string, DDL_BufferType> GEX_IndexArray::PropertyList::typeProperties
{
	{ "material", PropertyList::MATERIAL},
	{ "restart",PropertyList::RESTART},
	{ "front",PropertyList::FRONT},
};

DDL_Structure* GEX_IndexArray::Build(const std::string& partitionedStructure, DDL_ReferenceMap& referenceMap)
{
	auto result = New<DDL_Structure, ScratchPad<DDL_Structure>>(1);
	for (const DDL_Property& property : PartitionStructureProperties(ParseStructureHeader(partitionedStructure, *result)))
		switch (PropertyList::typeProperties.find(property.key.c_str())->second)
		{
		case PropertyList::MATERIAL:
			material = std::stoi(property.value.c_str());
			break;
		case PropertyList::RESTART:
		{
			char* end;
			restart = std::strtoull(property.value.c_str(), &end, 10);
			break;
		}
		case PropertyList::FRONT:
			front = property.value;
			break;
			DEFAULT_BREAK
		}
	result->subSutructres.emplace_back(indexArray.Build(PartitionDDLSubStructuresV2({partitionedStructure.c_str()})[0].c_str(), referenceMap));//should only have 1
	result->typeHeap = static_cast<void*>(this);
	MapStructureName(*result, referenceMap);

	return result;
}

 const std::map<std::string, DDL_BufferType> GEX_Mesh::PropertyList::typeProperties
{
	{ "lod", PropertyList::LOD},
	{ "primitive",PropertyList::PRIMTIIVE },
};

DDL_Structure* GEX_Mesh::Build(const std::string& partitionedStructure, DDL_ReferenceMap& referenceMap)
{
	vertexArrays.reserve(VECTOR_RESERVATION_SIZE);
	indexArrays.reserve(VECTOR_RESERVATION_SIZE);

	auto result = New<DDL_Structure, ScratchPad<DDL_Structure>>(1);	
	for (const DDL_Property& property : PartitionStructureProperties(ParseStructureHeader(partitionedStructure, *result)))
		switch (PropertyList::typeProperties.find(property.key.c_str())->second)
		{
			case PropertyList::LOD:
				lod = std::stoi(property.value.c_str());
				break;
			case PropertyList::PRIMTIIVE:
				primitive = property.value;
				break;
			DEFAULT_BREAK
		}			
	for (const auto& subStructure : PartitionDDLSubStructuresV2({ partitionedStructure.c_str()}))
		return result;
	auto subStructure = "subStructure";
	switch (ExtractStructureType(subStructure))
		{			
			case GEX_BufferTypes::GEX_VertexArray:				
				vertexArrays.emplace_back();
				result->subSutructres.emplace_back(vertexArrays.back().Build(subStructure, referenceMap));
				break;
			case GEX_BufferTypes::GEX_IndexArray:
				indexArrays.emplace_back();
				result->subSutructres.emplace_back(indexArrays.back().Build(subStructure, referenceMap));
				break;
			case GEX_BufferTypes::GEX_Skin:
				result->subSutructres.emplace_back(((skin = new GEX_Skin)->Build(subStructure, referenceMap)));
				break;
			DEFAULT_BREAK
		}
	result->typeHeap = static_cast<void*>(this);
	MapStructureName(*result, referenceMap);
	
	return result;
}