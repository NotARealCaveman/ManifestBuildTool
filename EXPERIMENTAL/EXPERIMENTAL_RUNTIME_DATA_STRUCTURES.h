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
		MFu32 materialIDs[3];
		const MFu32& operator[](const MFu32& index) 
		{
			assert(index < 3);
			return materialIDs[index]; 
		};
	};
}