#pragma once

#include <ManifestGLPersistence/MDB/Binary/Binary_Types.h>

namespace Manifest_Persistence
{
	struct ManifestOfflineDatabase
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
		ColorTable colorTable;
	};	

	void ExportOfflineDatabase(const ManifestOfflineDatabase& database);

	struct ManifestRuntimeDatabase
	{
		BinaryGeometryNodeTable binaryGeometryNodeTable;
		BinaryGeometryObjectTable binaryGeometryObjectTable;
		BinaryMeshTable binaryMeshTable;
	};
}
