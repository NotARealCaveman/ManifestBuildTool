#include "Manifest_Texture.h"

using namespace Manifest_Persistence;

void Manifest_Persistence::TableEntry(const DDL_Structure& structure, const ForeignKey& materialID, TextureTable& textureTable)
{	
	MDB_Texture& entry = textureTable.entries.emplace_back();
	entry.textureID = textureTable.nextTableIndex++;
	textureTable.mappedEntryKeys.insert({ structure.name,entry.textureID });
	switch (DDL_BufferTypes::DDL_BufferTypeMap.find(structure.identifier)->second)
	{
		case GEX_BufferTypes::GEX_Color:
		{
			const auto& color{ HeapData<GEX_Color>(structure) };
			entry.channelElements = entry.nChannels = color.colorType == GEX_ColorType::RGBA ? 4 : 3;
			entry.channelData = new float[entry.nChannels];
			memcpy(entry.channelData, color.channel.data.typeHeap, entry.nChannels * sizeof(float));
			break;
		}
		case GEX_BufferTypes::GEX_Texture:
			break;
		DEFAULT_BREAK
	}
}