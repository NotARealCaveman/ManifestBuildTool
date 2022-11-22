#pragma once
#include <vector>
#include <string>
#include <map>

#include <ManifestGLParser/OpenDDL/DDL_Structure.h>


namespace Manifest_Parser
{
	struct RegisteredBuilder
	{								
		virtual DDL_Structure Build(const std::string& partitionedStructure, DDL_ReferenceMap& referenceMap) = 0;
	};		
}