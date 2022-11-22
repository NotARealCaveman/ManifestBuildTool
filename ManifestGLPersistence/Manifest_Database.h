#pragma once
#include <ManifestGLPersistence/MDB/Manifest_GeometryNode.h>

namespace Manifset_Persistence
{

	struct ManifestDatabase
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
}
