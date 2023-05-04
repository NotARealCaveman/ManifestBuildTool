#pragma once

#include <ManifestParser/MDD/MDD_Collider.h>
#include <ManifestParser/DDL_ExtendedTypes.h>

#include "MDB_Table.h"

using namespace Manifest_Parser;

namespace Manifest_Persistence
{
	struct MDB_Collider
	{
		PrimaryKey colliderID;
		MFu8 colliderType;
		MFfloat* colliderData;
	};
	using ColliderBuildTable = DatabaseBuildTable<MDB_Collider>;
	ForeignKey TableEntry(const DDL_Structure& structure, ColliderBuildTable& colliderBuildTable);
}