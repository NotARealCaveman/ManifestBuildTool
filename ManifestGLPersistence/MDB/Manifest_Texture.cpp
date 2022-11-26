#include "Manifest_Texture.h"

using namespace Manifest_Persistence;

const std::map<std::string, MFu8> TextureTypes::textureTypeMap
{
	{ "diffuse", DIFFUSE_TEXTURE },
	{ "normal", NORMAL_TEXTURE },
	{ "parallax", PARALLAX_TEXTURE }
};

//TABLE INCLUDES COMPOSITE KEY: BITS 0-60 KEY WARD(size) BITS 61-63 KEY BOW(nChannels)
//TABLE INCLUDES COMPOSITE KEY: BITS 0-31 KEY WARD(width) BITS 32-63 KEY BOW(height)
void Manifest_Persistence::TableEntry(const DDL_Structure& structure, const ForeignKey& materialID, TextureTable& textureTable)
{	
	MDB_Texture& entry = textureTable.entries.emplace_back();
	entry.textureID = textureTable.nextTableIndex++;	
	textureTable.mappedEntryKeys.insert({ structure.name,entry.textureID });	
	entry.materialID = materialID;
	switch (DDL_BufferTypes::DDL_BufferTypeMap.find(structure.identifier)->second)
	{
		case GEX_BufferTypes::GEX_Color:
		{			
			const auto& color{ HeapData<GEX_Color>(structure) };
			const auto& size = color.colorType == GEX_ColorType::RGBA ? 4 : 3;
			SetCompositeBow(size, TEXTURE_INFO_BOW_BITOFFSET,entry.textureInfo);//sets number of channels in tetxure info
			SetCompositeWard(size, TEXTURE_INFO_BOW_BITOFFSET, entry.textureInfo);//sets number of elements in texture info
			SetCompositeBow(1, TEXTURE_DIMENSION_BOW_BITOFFSET, entry.textureDimensions);//sets the height of the texture
			SetCompositeWard(1, TEXTURE_DIMENSION_BOW_BITOFFSET, entry.textureDimensions);//sets the width of the texture
			entry.textureType = TextureTypes::textureTypeMap.find(color.attrib)->second;
			entry.channelData = new float[size];
			memcpy(entry.channelData, color.channel.data.typeHeap, size * sizeof(float));
			break;
		}
		case GEX_BufferTypes::GEX_Texture:
			break;
		DEFAULT_BREAK
	}
}