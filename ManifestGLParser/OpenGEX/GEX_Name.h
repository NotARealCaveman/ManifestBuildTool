#pragma once
#include <string>

#include <ManifestGLParser/RegisteredBuilders.h>
#include <ManifestGLParser/OpenDDL/DDL_Primitives.h>

namespace Manifest_Parser
{
	struct GEX_Name : public RegisteredBuilder
	{
		DDL_Structure Build(const ScratchPadString& partitionedStructure, DDL_ReferenceMap& referenceMap) final;
		DDL_String name;		
	};	
}