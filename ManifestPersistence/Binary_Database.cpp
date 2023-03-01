#include "Binary_Database.h"

using namespace Manifest_Persistence;

ManifestBinaryDatabase Manifest_Persistence::ImportGameDatabase(std::ifstream& importFile)
{
	ManifestBinaryDatabase result;

	result.binaryMeshTable = ImportBinaryTable<Binary_Mesh>(importFile);
	result.binaryMaterialTable = ImportBinaryTable<Binary_Material>(importFile);
	result.binaryTextureTable = ImportBinaryTable<Binary_Texture>(importFile);
	result.binaryGeometryObjectTable = ImportBinaryTable<Binary_GeometryObject>(importFile);
	result.binaryGeometryNodeTable = ImportBinaryTable<Binary_GeometryNode>(importFile);

	return result;
}

ManifestBinaryDatabase::ManifestBinaryDatabase(ManifestBinaryDatabase&& other)
	: binaryMeshTable{ std::move(other.binaryMeshTable) },
	binaryMaterialTable{ std::move(other.binaryMaterialTable) },
	binaryTextureTable{ std::move(other.binaryTextureTable) },
	binaryGeometryNodeTable{ std::move(other.binaryGeometryNodeTable) },
	binaryGeometryObjectTable{ std::move(other.binaryGeometryObjectTable) }
{}