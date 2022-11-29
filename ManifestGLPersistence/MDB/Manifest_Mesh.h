#pragma once

#include <ManifestGLPersistence/Manifest_Table.h>

#include "Manifest_VertexArray.h"
#include "Manifest_IndexArray.h"

namespace Manifest_Persistence
{
	
	struct MDB_Mesh
	{
		PrimaryKey meshID = KEY_NOT_PRESENT;		
		VertexArrayIDs vertexArrayIDs;
		ForeignKey indexArrayID;
	}; 
	typedef DatabaseBuildTable<MDB_Mesh> MeshBuildTable;
	ForeignKey TableEntry(const DDL_Structure& structure, MeshBuildTable& meshBuildTable, VertexBuildTables& vertexyBuildTables, IndexBuildTable& indexBuildTable);
}