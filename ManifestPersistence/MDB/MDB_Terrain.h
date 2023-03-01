#pragma once

#include <ManifestPersistence/DatabaseTypes.h>

#include "MDB_Table.h"

namespace Manifest_Persistence
{
	struct MDB_Terrain
	{
	};
	typedef DatabaseBuildTable<MDB_Terrain> TerrainBuildTable;
	ForeignKey TableEntry(TerrainBuildTable& terrainBuildTable );
}