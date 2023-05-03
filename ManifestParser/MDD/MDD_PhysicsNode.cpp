#include "MDD_PhysicsNode.h"

using namespace Manifest_Parser;

DDL_Structure* MDD_PhysicsNode::Build(const std::string_view& partitionedStructureView, DDL_ReferenceMap& referenceMap)
{
	auto result = New<DDL_Structure, ScratchPad<DDL_Structure>>(1);

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