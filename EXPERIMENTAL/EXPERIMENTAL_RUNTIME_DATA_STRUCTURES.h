#pragma once
#include <atomic>
#include <thread>

#include "ManifestUtility/Typenames.h"
#include <ManifestUtility/DebugLogger.h>

using namespace Manifest_Utility;

namespace Manifest_Experimental
{	
	struct Texture
	{
		MFu32  textureId;
	};

	struct Material
	{
		Texture materialTextures[3];
		const MFu32& operator[](const MFu32& index) 
		{
			assert(index < 3);
			return materialTextures[index].textureId;
		};
	};

	struct Mesh
	{
		MFu32 VAO;
		MFu32 VBO;
		MFu32 EBO;
		MFu32 indexCount;
	};

	struct GeometryObject
	{
		MFu32 meshId;
	};
}