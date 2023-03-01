#include "MDD_Terrain.h"

using namespace Manifest_Parser;

const std::map<std::string, DDL_BufferType> MDD_Terrain::PropertyList::typeProperties
{	
};

DDL_Structure* MDD_Terrain::Build(const std::string& partitionedStructure, DDL_ReferenceMap& referenceMap)
{
	auto result = New<DDL_Structure, ScratchPad<DDL_Structure>>(1);

	for (const DDL_Property& property : PartitionStructureProperties(ParseStructureHeader(partitionedStructure, *result)))
		switch (PropertyList::typeProperties.find(property.key.c_str())->second)
		{
			DEFAULT_BREAK;
		}
	for (const auto& subStructure : PartitionDDLSubStructures(partitionedStructure))
		switch (ExtractStructureType(subStructure))
		{
			case DDL_BufferTypes::DDL_uint8:
				result->subSutructres.emplace_back(index.Build(subStructure,referenceMap));
				break;
			case DDL_BufferTypes::DDL_int8:
				result->subSutructres.emplace_back(field.Build(subStructure, referenceMap));
				break;
			DEFAULT_BREAK
		}
	result->typeHeap = static_cast<void*>(this);
	MapStructureName(*result, referenceMap);

	return result;
}