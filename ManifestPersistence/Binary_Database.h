#pragma once

#include <ManifestPersistence/MDB/Binary/Binary_Types.h>

namespace Manifest_Persistence
{
	struct ManifestBinaryResourceDatabase
	{
		ManifestBinaryResourceDatabase() = default;
		ManifestBinaryResourceDatabase(const ManifestBinaryResourceDatabase&) = delete;
		ManifestBinaryResourceDatabase(ManifestBinaryResourceDatabase&& other);

		BinaryMeshTable binaryMeshTable;
		BinaryMaterialTable binaryMaterialTable;
		BinaryTextureTable binaryTextureTable;
		BinaryGeometryObjectTable binaryGeometryObjectTable;
		BinaryGeometryNodeTable binaryGeometryNodeTable;
	};
	ManifestBinaryResourceDatabase ImportGameResources(std::ifstream& resourceFile);

	struct ManifestBinaryTerrainDatabase
	{
		ManifestBinaryTerrainDatabase() = default;
		ManifestBinaryTerrainDatabase(const ManifestBinaryTerrainDatabase&) = delete;
		ManifestBinaryTerrainDatabase(ManifestBinaryTerrainDatabase&& other);
		
		BinaryVoxelMapTable binaryVoxelMapTable;
		BinaryTerrainTable binaryTerrainTable;		
	};
	ManifestBinaryTerrainDatabase ImportGameTerrain(std::ifstream& terrainFile);
}
