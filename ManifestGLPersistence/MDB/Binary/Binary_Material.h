#pragma once

#include <ManifestGLPersistence/MDB/Manifest_Material.h>

namespace Manifest_Persistence
{

	//TODO: AS A MORE SOPHISTICATED RENDERING MODEL IS DEVELOPED THE MATERIAL AND TEXTURE BINARIES/MDBS WILL BE REWORKED FOR THE PROPER MODEL. FOR THE TIME BEINIG THE DATA WILL SIMPLY LOAD THE COLOR DATA FROM THE GEX TEST FILE AND THEN IN A SECOND PASS DO A SLOPPY SET UP FOR THE CURRENT RENDERING ALGORITHM. NOT MANY OPTIMIZATIONS WILL BE MADE JUST A SIMPLE DATA TRANMISSION SO THE FRAMWORK CAN STILL BEGIN TO WORK WITH BINARY MATERIAL/TEXTURE DATA
	struct Binary_Material
	{
		struct Entry_Header
		{
			MFsize payloadSize{ 0 };//amount of raw bytes comprising the material texture data		
			MFsize normalOffset;//offset from diffuse to normal 
			MFsize parallaxOffset;//offset from normal to parallax
		}header;
		float* payload;//contains diffuse->normal->parallax material data
	};
	//see Buffer_Types.cpp for details
	size_t Convert_MDB(const MDB_Material& material, const TextureTable& textureTable, Binary_Material& binaryMaterial);
}