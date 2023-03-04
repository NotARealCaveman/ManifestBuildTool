#include "MDD_VoxelMap.h"

using namespace Manifest_Parser;

const std::map<std::string, DDL_BufferType> MDD_VoxelMap::PropertyList::typeProperties
{
	{"nVoxels",PropertyList::N_VOXELS},
	{"mVoxels",PropertyList::M_VOXELS},
	{"hVoxels",PropertyList::H_VOXELS}
};

DDL_Structure* MDD_VoxelMap::Build(const std::string& partitionedStructure, DDL_ReferenceMap& referenceMap)
{
	auto result = New<DDL_Structure, ScratchPad<DDL_Structure>>(1);

	for (const DDL_Property& property : PartitionStructureProperties(ParseStructureHeader(partitionedStructure, *result)))
		switch (PropertyList::typeProperties.find(property.key.c_str())->second)
		{
			case PropertyList::N_VOXELS:
				nVoxels = std::stoi(property.value.c_str());
				break;
			case PropertyList::M_VOXELS:
				mVoxels = std::stoi(property.value.c_str());
				break;
			case PropertyList::H_VOXELS:
				hVoxels = std::stoi(property.value.c_str());
				break;
			DEFAULT_BREAK;
		}
	for (const auto& subStructure : PartitionDDLSubStructures(partitionedStructure))
		switch (ExtractStructureType(subStructure))
		{
		case DDL_BufferTypes::DDL_int8:
			result->subSutructres.emplace_back(field.Build(subStructure, referenceMap));
			break;
			DEFAULT_BREAK
		}
	result->typeHeap = static_cast<void*>(this);
	MapStructureName(*result, referenceMap);

	return result;
}