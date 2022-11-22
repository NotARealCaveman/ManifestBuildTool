#pragma once

#include <ManifestGLParser/OpenGEX/GEX_GeometryObject.h>
#include <ManifestGLPersistence/Manifest_Table.h>

#include "Manifest_Mesh.h" 

using namespace Manifest_Parser;

namespace Manifset_Persistence
{
	struct MDB_GeometryObject
	{
		PrimaryKey geometryID = KEY_NOT_PRESENT;
		ForeignKey meshID = KEY_NOT_PRESENT;
		//const uint32_t morphID;		
	};
	typedef DatabaseTable<MDB_GeometryObject> GeometryObjectTable;
	ForeignKey TableEntry(const DDL_Structure& structure, GeometryObjectTable& geometryObjectTable, MeshTable& meshTable, VertexTables& vertexTables, IndexTable& indexTable);
	
}