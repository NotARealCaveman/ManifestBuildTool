#pragma once

#include <ManifestGLParser/OpenGEX/GEX_Color.h>
#include <ManifestGLPersistence/Manifest_Table.h>

using namespace Manifest_Parser;

namespace Manifest_Persistence
{
	//a MDB_Texture may be created from either a GEX_Color or GEX_Texture
	//the specific channel data will be stored in the channelData member which converts to the payload for the binary type conversion	
	struct MDB_Texture
	{
		PrimaryKey textureID = KEY_NOT_PRESENT;
		ForeignKey materialID = KEY_NOT_PRESENT;
		uint8_t nChannels = CHANNELS_NOT_PRESENT;
		size_t channelElements = CHANNELS_NOT_PRESENT;//when the texture is a color matches number of channels, when texture is an image equals number of float elements in the image channels
		float* channelData = BUFFER_NOT_PRESENT;
	};
	typedef DatabaseTable<MDB_Texture> TextureTable;
	void TableEntry(const DDL_Structure& structure,const ForeignKey& materialID, TextureTable& textureTable);
}