#pragma once

#include <ManifestParser/MDD/MDD_Terrain.h>
#include <ManifestParser/DDL_ExtendedTypes.h>

#include <ManifestPersistence/DatabaseTypes.h>
#include "MDB_Table.h"

using namespace Manifest_Parser;

namespace Manifest_Persistence
{
	struct MDB_Terrain
	{
		PrimaryKey terrainID;
		MFint8* terrainSDF;
		MFsize SDFSampleSize;		
	};
	using TerrainBuildTable = DatabaseBuildTable<MDB_Terrain>;
	ForeignKey TableEntry(const DDL_Structure&  structure, TerrainBuildTable& terrainBuildTable);
}