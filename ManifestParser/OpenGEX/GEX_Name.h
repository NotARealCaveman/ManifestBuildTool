#pragma once
#include <string>

#include <ManifestParser/RegisteredBuilders.h>
#include <ManifestParser/OpenDDL/DDL_Primitives.h>

namespace Manifest_Parser
{
	struct GEX_Name : public RegisteredBuilder
	{
		DDL_Structure* Build(const std::string_view& partitionedStructureView, DDL_ReferenceMap& referenceMap) final;
		DDL_String name;		
	};	
}