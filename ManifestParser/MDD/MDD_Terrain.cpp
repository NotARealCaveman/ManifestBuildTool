#include "MDD_Terrain.h"

using namespace Manifest_Parser;

const std::map<std::string, DDL_BufferType> MDD_Terrain::PropertyList::typeProperties
{	
	{"xChunkIndex",PropertyList::X_CHUNK_INDEX},
	{"zChunkIndex",PropertyList::Z_CHUNK_INDEX}
};

DDL_Structure* MDD_Terrain::Build(const std::string_view& partitionedStructureView, DDL_ReferenceMap& referenceMap)
{
	auto result = New<DDL_Structure, ScratchPad<DDL_Structure>>(1);

	for (const DDL_Property& property : PartitionStructureProperties(ParseStructureHeader(partitionedStructureView, *result)))
		switch (PropertyList::typeProperties.find(static_cast<std::string>(property.key))->second)
		{
			case PropertyList::X_CHUNK_INDEX:
				xChunkIndex = std::stoi(static_cast<std::string>(property.value));
				break;
			case PropertyList::Z_CHUNK_INDEX:
				zChunkIndex = std::stoi(static_cast<std::string>(property.value));
				break;
			DEFAULT_BREAK;
		}
	for (const auto& subStructure : PartitionDDLSubStructures(partitionedStructureView))
		switch (ExtractStructureType(subStructure))
		{
			case DDL_BufferTypes::DDL_uint8:
				result->subSutructres.emplace_back(index.Build(subStructure,referenceMap));
				break;
			DEFAULT_BREAK
		}
	result->typeHeap = static_cast<void*>(this);
	MapStructureName(*result, referenceMap);

	return result;
}