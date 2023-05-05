#include "MDB_PhysicsNode.h"

using namespace Manifest_Persistence;

ForeignKey Manifest_Persistence::TableEntry(const DDL_Structure& structure, PhysicsNodeBuildTable& physicsNodeBuildTable, RigidbodyBuildTable& rigidBodyBuildTable, ColliderBuildTable& colliderBuildTable)
{
	MDB_PhysicsNode& entry = physicsNodeBuildTable.entries.emplace_back();
	entry.colliderID = colliderBuildTable.nextTableIndex++;
	colliderBuildTable.mappedEntryKeys.insert({ structure.name.c_str(), entry.colliderID });

	for (const auto& substructure : structure.subSutructres)
	{
		switch (ExtractStructureType(substructure->identifier.c_str()))
		{
			case DDL_ExtendedTypes::MDD_RIGIDBODYPARAMS:
				entry.rigidBodyID= TableEntry(*substructure, rigidBodyBuildTable);
				break;
			case DDL_ExtendedTypes::MDD_COLLIDER:
				entry.colliderID = TableEntry(*substructure, colliderBuildTable);
				break;
			DEFAULT_BREAK
		}
	}
	

	return entry.colliderID;
}