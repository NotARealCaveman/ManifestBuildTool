#pragma once

#include <ManifestGLParser/OpenGEX/GEX_Mesh.h>
#include <ManifestGLPersistence/Manifest_Table.h>

using namespace Manifest_Parser;

namespace Manifset_Persistence
{
	struct MDB_VertexArray
	{
		PrimaryKey vertexArrayID =	KEY_NOT_PRESENT;
		float* vertexData = BUFFER_NOT_PRESENT;				
		size_t elements = 0;
	};

	typedef DatabaseTable<MDB_VertexArray> VertexArrayTable;
	ForeignKey TableEntry(const DDL_Structure& structure, VertexArrayTable& vertexArrayTable);
	typedef VertexArrayTable VertexTable;
	typedef VertexArrayTable UVTable;
	typedef VertexArrayTable NormalTable;
	typedef VertexArrayTable TangentTable;
	typedef VertexArrayTable BitangentTable;

	struct VertexTables
	{
		VertexTable vertexTable;
		UVTable uvTable;
		NormalTable normalTable;
		TangentTable tangentTable;
		BitangentTable bitangentTable;;
	};
	struct VertexArrayIDs
	{
		ForeignKey vertexID = KEY_NOT_PRESENT;
		ForeignKey uvID = KEY_NOT_PRESENT;
		ForeignKey normalID = KEY_NOT_PRESENT;
		ForeignKey tangentID = KEY_NOT_PRESENT;
		ForeignKey bitangentID = KEY_NOT_PRESENT;;
	};
}