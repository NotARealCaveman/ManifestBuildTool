#include "Manifest_Color.h"

using namespace Manifest_Persistence;

ForeignKey Manifest_Persistence::TableEntry(const DDL_Structure& structure, ColorTable& colorTable)
{	
	MDB_Color& entry = colorTable.entries.emplace_back();
	entry.colorID = colorTable.nextTableIndex++;
	colorTable.mappedEntryKeys.insert({ structure.name,entry.colorID });
	const auto& color{ HeapData<GEX_Color>(structure) };
	entry.nChannels = color.colorType == GEX_ColorType::RGBA ? 4 : 3;
	entry.channelData = new float[entry.nChannels];
	memcpy(entry.channelData, color.channel.data.typeHeap, entry.nChannels * sizeof(float));
	return entry.colorID;
}