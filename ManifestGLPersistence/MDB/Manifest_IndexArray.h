#pragma once

#include <ManifestGLParser/OpenGEX/GEX_Mesh.h>
#include <ManifestGLPersistence/Manifest_Table.h>

using namespace Manifest_Parser;

namespace Manifset_Persistence
{
	struct MDB_IndexArray
	{
		PrimaryKey indexArrayID = KEY_NOT_PRESENT;
		uint32_t* indexData = BUFFER_NOT_PRESENT;
		size_t elements = 0;
	};
	typedef DatabaseTable<MDB_IndexArray> IndexTable;
	ForeignKey TableEntry(const DDL_Structure& structure, IndexTable& indexTable);
}