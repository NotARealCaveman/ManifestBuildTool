#include "MDD_Collider.h"

using namespace Manifest_Parser;

const std::map<std::string, DDL_BufferType>
MDD_Collider::PropertyList::typeProperties
{
	{"shape", PropertyList::SHAPE}
};

DDL_Structure* MDD_Collider::Build(const std::string_view& partitionedStructureView, DDL_ReferenceMap& referenceMap)
{
	auto result = New<DDL_Structure, ScratchPad<DDL_Structure>>(1);
	ParseStructureHeader(partitionedStructureView, *result);
	for (const DDL_Property& property : PartitionStructureProperties(ParseStructureHeader(partitionedStructureView, *result)))
		switch (PropertyList::typeProperties.find(static_cast<std::string>(property.key))->second)
		{
			case PropertyList::SHAPE:
			{
				switch (std::stoi(static_cast<std::string>(property.value)))
				{
					case COLLIDER_GEOMETRY::SPHERE:
						this->colliderType = COLLIDER_GEOMETRY::SPHERE;
						break;
					case COLLIDER_GEOMETRY::CAPSULE:
						this->colliderType = COLLIDER_GEOMETRY::CAPSULE;
						break;
					case COLLIDER_GEOMETRY::CONVEXHULL:
						this->colliderType = COLLIDER_GEOMETRY::CONVEXHULL;
						break;
				}
				break;
			};			
			DEFAULT_BREAK;
		}
	for (const auto& subStructure : PartitionDDLSubStructures(partitionedStructureView))
		switch (ExtractStructureType(subStructure))
		{
			case DDL_BufferTypes::DDL_float:
			{				
				colliderData.Build(subStructure, referenceMap);
				break;
			}
			DEFAULT_BREAK
		}
	result->typeHeap = static_cast<void*>(this);
	MapStructureName(*result, referenceMap);

	return result;
}