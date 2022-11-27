#pragma once 

#include <ManifestGLPersistence/MDB/Manifest_Texture.h>

namespace Manifest_Persistence
{	
	struct Binary_Texture
	{
		struct Entry_Header
		{
			MFsize payloadSize{ 0 };//stores size of texture data in bytes					
			MFu32 width;
			MFu32 height;
			MFu32 internalFormat;
			MFu32 dataFormat;
			MFu8 nChannels;
			MFu8 textureType;//type of texture the map represents			
		}header;
		float* payload;
	};
	//see Buffer_Types.cpp for details
	size_t Convert_MDB(const MDB_Texture& texture, Binary_Texture& binaryTexture);
}