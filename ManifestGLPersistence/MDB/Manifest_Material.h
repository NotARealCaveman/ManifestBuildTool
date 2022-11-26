#pragma once

#include <ManifestGLParser/OpenGEX/GEX_Material.h>
#include <ManifestGLPersistence/Manifest_Table.h>

#include "Manifest_Texture.h"

namespace Manifest_Persistence
{
	struct MDB_Material
	{
		PrimaryKey materialID = KEY_NOT_PRESENT;		
	};
	typedef DatabaseTable<MDB_Material> MaterialTable;
	ForeignKey TableEntry(const DDL_Structure& structure, MaterialTable& materialTable, TextureTable& colorTable);
}