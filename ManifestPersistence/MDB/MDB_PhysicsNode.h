#pragma once

#include <ManifestParser/MDD/MDD_PhysicsNode.h>
#include <ManifestParser/DDL_ExtendedTypes.h>

#include "MDB_Collider.h"
#include "MDB_Rigidbody.h"

#include "MDB_Table.h"

using namespace Manifest_Parser;

namespace Manifest_Persistence
{
	struct MDB_PhysicsNode
	{
		PrimaryKey physicsNodeID;
		ForeignKey rigidBodyID;
		ForeignKey colliderID;
	};
	using PhysicsNodeBuildTable = DatabaseBuildTable<MDB_PhysicsNode>;
	ForeignKey TableEntry(const DDL_Structure& structure, PhysicsNodeBuildTable& physicsNodeBuildTable,RigidbodyBuildTable& rigidBodyBuildTable, ColliderBuildTable& colliderBuildTable);
}