#pragma once

#include <ManifestParser/OpenDDL/DDL_Buffers.h>

namespace Manifest_Parser
{
	struct DDL_ExtendedTypes
	{	
		static constexpr DDL_BufferType MDD_TERRAIN{ DDL_BufferTypes::NEXT_BUFFER_ID + 1 };
		static constexpr DDL_BufferType MDD_VOXELMAP{ MDD_TERRAIN + 1 };
		static constexpr DDL_BufferType MDD_PHYSICSNODE{ MDD_VOXELMAP + 1 };
		static constexpr DDL_BufferType MDD_RIGIDBODYPARAMS{ MDD_PHYSICSNODE + 1 };
		static constexpr DDL_BufferType MDD_COLLIDER{ MDD_RIGIDBODYPARAMS + 1 };
		static constexpr DDL_BufferType NEXT_BUFFER_ID{ MDD_COLLIDER + 1 };
	}; 		
}