#pragma once

#include <ManifestGLPersistence/MDB/Binary/Binary_Types.h>

namespace Manifest_Persistence
{
	struct ManifestDatabaseBuild
	{
		GeometryObjectTable geometryObjectTable;
		MeshTable meshTable;
		VertexTables vertexTables;
		IndexTable indexTable;
		GeometryNodeTable geometryNodeTable;
		ObjectRefTable objectRefTable;
		MaterialRefTable materialRefTable;
		MaterialTable materialTable;
		MaterialSwitchTable materialSwitchTable;
		TextureTable textureTable;
	};	

	void ExportRuntimeDatabase(const ManifestDatabaseBuild& databaseBuild, std::ofstream& exportFile);

	struct ManifestRuntimeDatabase
	{
		ManifestRuntimeDatabase() = default;
		ManifestRuntimeDatabase(const ManifestRuntimeDatabase&) = delete;
		ManifestRuntimeDatabase(ManifestRuntimeDatabase&& other);		
				
		BinaryMeshTable binaryMeshTable;		
		BinaryTextureTable binaryTextureTable;
		BinaryGeometryObjectTable binaryGeometryObjectTable;
		BinaryGeometryNodeTable binaryGeometryNodeTable;		
	};

	ManifestRuntimeDatabase ImportRuntimeDatabase(std::ifstream& importFile);
}
