#pragma once
#include <map>
#include <string>

#include <ManifestParser/OpenDDL/DDL_Primitives.h>

namespace Manifest_Parser
{
	struct MDD_Terrain : public RegisteredBuilder
	{		
		struct PropertyList 
		{		
			static constexpr uint32_t X_CHUNK_INDEX{ 0 };
			static constexpr uint32_t Z_CHUNK_INDEX{ X_CHUNK_INDEX + 1 };
			static const std::map<std::string, DDL_BufferType> typeProperties;
		};
		DDL_Structure* Build(const std::string_view& partitionedStructureView, DDL_ReferenceMap& referenceMap)final;//tbd
		//substructures
		DDL_Uint8 index;
		//properties
		MFu32 xChunkIndex{ UINT32_MAX };
		MFu32 zChunkIndex{ UINT32_MAX };
	};
}