#pragma once

#include "Binary_Database.h"

namespace Manifest_Persistence
{
	struct ManifestDatabaseBuilder
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

	void ExportBinaryDatabase(const ManifestDatabaseBuilder& databaseBuild, std::ofstream& exportFile);
}
