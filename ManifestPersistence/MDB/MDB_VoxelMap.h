#pragma once

#include <ManifestParser/MDD/MDD_VoxelMap.h>
#include <ManifestParser/DDL_ExtendedTypes.h>

#include "MDB_Table.h"

using namespace Manifest_Parser;

namespace Manifest_Persistence
{	
	struct MDB_VoxelMap
	{
		PrimaryKey mapID;
		MFint8* mapSDF;
		MFu32 nVoxels;
		MFu32 mVoxels;
		MFu32 hVoxels;
	};
	using VoxelMapBuildTable = DatabaseBuildTable<MDB_VoxelMap>;
	ForeignKey TableEntry(const DDL_Structure& structure, VoxelMapBuildTable& voxelMapBuildTable);
}