#pragma once

#include <ManifestGLParser/OpenGEX/GEX_Color.h>
#include <ManifestGLPersistence/Manifest_Table.h>

using namespace Manifest_Parser;

namespace Manifest_Persistence
{
	struct MDB_Color
	{
		PrimaryKey colorID = KEY_NOT_PRESENT;
		uint8_t nChannels = CHANNELS_NOT_PRESENT;
		float* channelData = BUFFER_NOT_PRESENT;//4th channel is optional
	};
	typedef DatabaseTable<MDB_Color> ColorTable;
	ForeignKey TableEntry(const DDL_Structure& structure, ColorTable& colorTable);
}