#pragma once
#include <vector>
#include <string>
#include <map>

#include <ManifestParser/OpenDDL/DDL_Structure.h>


namespace Manifest_Parser
{
	struct RegisteredBuilder
	{								
		virtual DDL_Structure* Build(const std::string_view& partitionedStructureView, DDL_ReferenceMap& referenceMap) = 0;
	};			
}