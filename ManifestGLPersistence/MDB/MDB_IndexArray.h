#pragma once

#include <ManifestGLParser/OpenGEX/GEX_Mesh.h>
#include <ManifestGLPersistence/Manifest_Table.h>

using namespace Manifest_Parser;

namespace Manifest_Persistence
{
	struct MDB_IndexArray
	{
		PrimaryKey indexArrayID = KEY_NOT_PRESENT;
		uint32_t* indexData = BUFFER_NOT_PRESENT;
		size_t elements = 0;
	};
	typedef DatabaseBuildTable<MDB_IndexArray> IndexBuildTable;
	ForeignKey TableEntry(const DDL_Structure& structure, IndexBuildTable& indexBuildTable);
}