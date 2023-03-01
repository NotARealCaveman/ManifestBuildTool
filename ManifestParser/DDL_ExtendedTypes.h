#pragma once

#include <ManifestParser/OpenDDL/DDL_Buffers.h>

namespace Manifest_Parser
{
	struct DDL_ExtendedTypes
	{	
		static constexpr DDL_BufferType MDD_TERRAIN{ DDL_BufferTypes::NEXT_BUFFER_ID + 1 };		
		static constexpr DDL_BufferType NEXT_BUFFER_ID{ MDD_TERRAIN + 1 };
	}; 		
}