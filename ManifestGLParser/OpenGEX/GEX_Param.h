#pragma once
#include <string>

#include <ManifestGLParser/OpenDDL/DDL_Primitives.h>

namespace Manifest_Parser
{
	struct GEX_Param : public RegisteredBuilder
	{
		struct PropertyList {
			static constexpr int ATTRIB{ 0 };
			static const std::map<std::string, uint32_t> typeProperties;
		};
		DDL_Structure* Build(const std::string& partitionedStructure, DDL_ReferenceMap& referenceMap) final;
		//substructures
		DDL_Float param;
		//properties
		ScratchPadString attrib;
	};
}
