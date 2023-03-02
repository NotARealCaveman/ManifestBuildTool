#pragma once

#include "Binary_Database.h"

namespace Manifest_Persistence
{
	struct ManifestResourceDatabaseBuilder
	{
		GeometryObjectBuildTable geometryObjectBuildTable;
		MeshBuildTable meshBuildTable;
		VertexBuildTables vertexBuildTables;
		IndexBuildTable indexBuildTable;
		GeometryNodeBuildTable geometryNodeBuildTable;
		ObjectRefBuildTable objectRefBuildTable;
		MaterialRefBuildTable materialRefBuildTable;
		MaterialBuildTable materialBuildTable;
		TextureBuildTable textureBuildTable;
	};	
	void ExportBinaryResourceDatabase(const ManifestResourceDatabaseBuilder& databaseBuild, std::ofstream& exportFile);

	struct ManifestTerrainDatabaseBuilder
	{
		TerrainBuildTable terrainBuildTable;
	};

	void ExportBinaryTerrainDatabase(const ManifestTerrainDatabaseBuilder& databaseBuild, std::ofstream& exportFile);
}
