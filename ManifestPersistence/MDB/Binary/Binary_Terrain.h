#pragma once

#include <ManifestPersistence/MDB/MDB_Terrain.h>

namespace Manifest_Persistence
{
	struct Binary_Terrain
	{
		struct Entry_Header
		{
			MFsize payloadSize{ 0 };//stores size of terrain data - to be done in rle	
			MFu8 terrainIndex;
		}header;
		MFint8* payload;//terrain SDF
	};	
	size_t Convert_MDB(const MDB_Terrain& terrain, Binary_Terrain& binaryTerrain);
}
