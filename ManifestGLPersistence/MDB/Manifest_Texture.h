#pragma once

#include <ManifestGLParser/OpenGEX/GEX_Color.h>
#include <ManifestGLPersistence/Manifest_Table.h>

using namespace Manifest_Parser;

namespace Manifest_Persistence
{
	//a MDB_Texture may be created from either a GEX_Color or GEX_Texture
	//the specific channel data will be stored in the channelData member which converts to the payload for the binary type conversion	

	constexpr MFu64 TEXTURE_BOW_BITOFFSET{ 61 };

	struct MDB_Texture
	{
		PrimaryKey textureID = KEY_NOT_PRESENT;
		ForeignKey materialID = KEY_NOT_PRESENT;
		CompositeKey textureInfo = KEY_NOT_PRESENT;//represents size(b:0-60) and nChannels(b:61-63)
		float* channelData = BUFFER_NOT_PRESENT;
	};
	typedef DatabaseTable<MDB_Texture> TextureTable;
	void TableEntry(const DDL_Structure& structure,const ForeignKey& materialID, TextureTable& textureTable);
}