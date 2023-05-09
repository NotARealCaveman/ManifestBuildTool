#include "MDB_GameObject.h"

using namespace Manifest_Persistence;

void Manifest_Persistence::TableEntry(const ScratchPadVector<DDL_Structure*>& gameObjects, const ScratchPadVector<DDL_Structure*>& geometryNodes, const ScratchPadVector<DDL_Structure*>& materials, const ScratchPadVector<DDL_Structure*>& geoemtryObjects)
{
	std::unordered_set<std::string> encounteredNodes;
	for (const auto& object : gameObjects)
	{
		const auto& gameObject{ HeapData<MDD_GameObject>(*object) };
		//get geometry node that belongs to this object
		const auto& ref{ gameObject.objectReferences.referenceNames[1] };
		GEX_GeometryNode* geometryNode{ nullptr };
		for(const auto& node : geometryNodes)
			if (node->name == ref)
			{
				geometryNode = reinterpret_cast<GEX_GeometryNode*>(node->typeHeap);
				break;
			}
		//check if object was found
		if (!geometryNode)
			continue;

	}
}

void Manifest_Persistence::TableEntry(const ScratchPadVector<DDL_Structure*>& gameObjects, const ScratchPadVector<DDL_Structure*>& physicsNodes)
{

}