#pragma once

#include <ManifestGLParser/OpenGEX/GEX_Material.h>
#include <ManifestGLPersistence/Manifest_Table.h>

#include "Manifest_Texture.h"

namespace Manifest_Persistence
{
	struct MDB_Material
	{
		PrimaryKey materialID = KEY_NOT_PRESENT;
		ForeignKey materialCode = KEY_NOT_PRESENT;
	};
	typedef DatabaseTable<MDB_Material> MaterialTable;
	ForeignKey TableEntry(const DDL_Structure& structure, MaterialTable& materialTable, TextureTable& colorTable);

	struct MDB_MaterialSwitch
	{
		PrimaryKey materialCode = KEY_NOT_PRESENT;
		//composite row, bool row is (0)color/(1)texture,int row is ID		
		CompositeKey tableCode = KEY_NOT_PRESENT;
	};		
	typedef DatabaseTable<MDB_MaterialSwitch> MaterialSwitchTable;
	//ForeignKey TableEntry(const GEX_Material& material, PrimaryKey& tableIndexIncrement);
}