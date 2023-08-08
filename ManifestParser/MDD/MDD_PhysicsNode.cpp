#include "MDD_PhysicsNode.h"

using namespace Manifest_Parser;


const std::map<std::string, DDL_BufferType> MDD_PhysicsNode::PropertyList::typeProperties
{
	{ "dynamic", PropertyList::DYNAMIC},
};

DDL_Structure* MDD_PhysicsNode::Build(const std::string_view& partitionedStructureView, DDL_ReferenceMap& referenceMap)
{
	auto result = New<DDL_Structure, ScratchPad<DDL_Structure>>(1);
	for (const DDL_Property& property : PartitionStructureProperties(ParseStructureHeader(partitionedStructureView, *result)))
		switch (PropertyList::typeProperties.find(static_cast<std::string>(property.key))->second)
		{
		case PropertyList::DYNAMIC:		
			std::stringstream{ static_cast<std::string>(property.value) } >> std::boolalpha >> isDynamic;
			break;
			DEFAULT_BREAK
		}	
	for (const auto& subStructure : PartitionDDLSubStructures(partitionedStructureView))
		switch (ExtractStructureType(subStructure))
		{			
			case DDL_ExtendedTypes::MDD_RIGIDBODYPARAMS:
			{
				rigidBodyParams.Build(subStructure, referenceMap);
				break;
			}
			case DDL_ExtendedTypes::MDD_COLLIDER:
			{
				collider.Build(subStructure, referenceMap);
				break;
			}
			DEFAULT_BREAK
		}
	result->typeHeap = static_cast<void*>(this);
	MapStructureName(*result, referenceMap);

	return result;
}