#pragma once

#include <ManifestPersistence/MDB/Binary/Binary_Types.h>

namespace Manifest_Persistence
{
	struct ManifestBinaryDatabase
	{
		ManifestBinaryDatabase() = default;
		ManifestBinaryDatabase(const ManifestBinaryDatabase&) = delete;
		ManifestBinaryDatabase(ManifestBinaryDatabase&& other);

		BinaryMeshTable binaryMeshTable;
		BinaryMaterialTable binaryMaterialTable;
		BinaryTextureTable binaryTextureTable;
		BinaryGeometryObjectTable binaryGeometryObjectTable;
		BinaryGeometryNodeTable binaryGeometryNodeTable;
	};
	ManifestBinaryDatabase ImportGameDatabase(std::ifstream& importFile);

	struct ManifestBinaryTerrain
	{

	};
	ManifestBinaryDatabase ImportGameTerrain(std::ifstream& importFile);
}
