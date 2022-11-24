#pragma once 

#include <ManifestGLPersistence/MDB/Manifest_Texture.h>

namespace Manifest_Persistence
{
	struct Binary_Texture
	{
		struct Entry_Header
		{
			size_t payloadSize{ 0 };//stores size of texture data in bytes		
			uint8_t nChannels;//number of chanels used by the terxture			
		}header;
		float* payload;
	};
	//see Buffer_Types.cpp for details
	size_t Convert_MDB(const MDB_Texture& texture, Binary_Texture& binaryTexture);
}