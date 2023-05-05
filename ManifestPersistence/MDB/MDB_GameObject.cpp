#include "MDB_GameObject.h"

using namespace Manifest_Persistence;

ForeignKey Manifest_Persistence::TableEntry(const DDL_Structure& structure, RigidBodyObjectBuildTable& rigidBodyObjectBuildTable, PhysicsNodeBuildTable& physicsNodeBuildTable, GeometryNodeBuildTable& geometryNodeBuildTable, RefBuildTable& physicsNodeRefBuildTable, RefBuildTable& graphicsNodeRefBuildTable)
{
	MDB_RigidBodyObject& entry = rigidBodyObjectBuildTable.entries.emplace_back();
	entry.rigidBodyObjectID = rigidBodyObjectBuildTable.nextTableIndex++;
	rigidBodyObjectBuildTable.mappedEntryKeys.insert({ structure.name.c_str(), entry.rigidBodyObjectID });

	const auto substructures{ structure.subSutructres };
	entry.physicsNodeRefID = TableEntry<MDB_PhysicsNode, MDD_PhysicsNode>(*substructures[0], physicsNodeBuildTable,physicsNodeRefBuildTable);
	entry.graphicsNodeRefID = TableEntry<MDB_GeometryNode, GEX_GeometryNode>(*substructures[1], geometryNodeBuildTable, graphicsNodeRefBuildTable);

	return entry.rigidBodyObjectID;
}
