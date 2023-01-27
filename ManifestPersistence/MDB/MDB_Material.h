#pragma once

#include <ManifestParser/OpenGEX/GEX_Material.h>
#include <ManifestPersistence/Manifest_Table.h>

#include "MDB_Texture.h"

namespace Manifest_Persistence
{
	struct MDB_Material
	{
		PrimaryKey materialID = KEY_NOT_PRESENT;		
	};
	typedef DatabaseBuildTable<MDB_Material> MaterialBuildTable;
	ForeignKey TableEntry(const DDL_Structure& structure, MaterialBuildTable& materialBuildTable, TextureBuildTable& textureBuildTable);
}