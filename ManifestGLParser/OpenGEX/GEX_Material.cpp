#include "GEX_Material.h"

using namespace Manifest_Parser;


const std::map<std::string, DDL_BufferType> GEX_Material::PropertyList::typeProperties
{
	{ "two_sided", PropertyList::TWOSIDED},	
};

DDL_Structure* GEX_Material::Build(const std::string& partitionedStructure, DDL_ReferenceMap& referenceMap)
{
	colors.reserve(VECTOR_RESERVATION_SIZE);
	parameters.reserve(VECTOR_RESERVATION_SIZE);
	spectrums.reserve(VECTOR_RESERVATION_SIZE);
	textures.reserve(VECTOR_RESERVATION_SIZE);

	auto result = New<DDL_Structure, ScratchPad<DDL_Structure>>(1);
	for (const DDL_Property& property : PartitionStructureProperties(ParseStructureHeader(partitionedStructure, *result)))
		switch (PropertyList::typeProperties.find(property.key)->second)
		{
		case PropertyList::TWOSIDED:
			std::stringstream{ property.value } >> std::boolalpha >> two_sided;
			break;
		default:
			break;
		}
	for (const auto& subStructure : PartitionDDLSubStructures(partitionedStructure))
		switch (ExtractStructureType(subStructure))
		{
			case GEX_BufferTypes::GEX_Name:
				result->subSutructres.emplace_back(name.Build(subStructure, referenceMap));
				break;
			case GEX_BufferTypes::GEX_Color:
				result->subSutructres.emplace_back(colors.emplace_back().Build(subStructure, referenceMap));
				break;
			case GEX_BufferTypes::GEX_Param:
				result->subSutructres.emplace_back(parameters.emplace_back().Build(subStructure, referenceMap));
				break;
			case GEX_BufferTypes::GEX_Spectrum:
				result->subSutructres.emplace_back(spectrums.emplace_back().Build(subStructure, referenceMap));
				break;
			case GEX_BufferTypes::GEX_Texture:
				result->subSutructres.emplace_back(textures.emplace_back().Build(subStructure, referenceMap));
				break;
		}
	result->typeHeap = static_cast<void*>(this);
	MapStructureName(*result, referenceMap);
	
	return result;
}

const std::map<std::string, DDL_BufferType> GEX_MaterialRef::PropertyList::typeProperties
{
	{ "index", PropertyList::INDEX},
};

DDL_Structure* GEX_MaterialRef::Build(const std::string& partitionedStructure, DDL_ReferenceMap& referenceMap)
{
	ReserveReferenceContainers(*this);

	auto result = New<DDL_Structure, ScratchPad<DDL_Structure>>(1);
	for (const DDL_Property& property : PartitionStructureProperties(ParseStructureHeader(partitionedStructure, *result)))
		switch (PropertyList::typeProperties.find(property.key)->second)
		{
			case PropertyList::INDEX:
				index = std::stoi(property.value);
				break;
			default:
				break;
		}
	for (const std::string& subStructure : PartitionDDLSubStructures(partitionedStructure))
		switch (ExtractStructureType(subStructure))
		{
			case DDL_BufferTypes::DDL_ref:
				result->subSutructres.emplace_back(DDL_Reference::Build(subStructure, referenceMap));
				MapStructureName(*(result->subSutructres.back()), referenceMap);
				break;
				DEFAULT_BREAK
		}
	result->typeHeap = static_cast<void*>(this);
	MapStructureName(*result, referenceMap);

	return result;
}