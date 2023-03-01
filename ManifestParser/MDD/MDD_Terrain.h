#pragma once
#include <map>
#include <string>

#include <ManifestParser/OpenDDL/DDL_Primitives.h>

namespace Manifest_Parser
{
	struct MDD_Terrain : public RegisteredBuilder
	{		
		struct PropertyList {			
			static const std::map<std::string, uint32_t> typeProperties;
		};
		DDL_Structure* Build(const std::string& partitionedStructure, DDL_ReferenceMap& referenceMap)final;//tbd
		//substructures
		DDL_Uint32 xBlocks;
		DDL_Uint32 zBlocks;
		DDL_Uint32 yBlocks;
		DDL_Int8 field;
	};
}