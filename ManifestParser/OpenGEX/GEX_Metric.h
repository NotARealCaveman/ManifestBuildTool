#pragma once
#include <vector>
#include <variant>
#include <string>

#include <ManifestParser/OpenDDL/DDL_Primitives.h>
#include <ManifestParser/DDL_ExtendedTypes.h>

namespace Manifest_Parser
{			

	struct GEX_Metric : public RegisteredBuilder
	{
		struct PropertyList {
			static constexpr int KEY{ 0 };			
			static const std::map<std::string, uint32_t> typeProperties;
		};
		DDL_Structure* Build(const std::string_view& partitionedStructureView, DDL_ReferenceMap& referenceMap) final;

		//structures
		std::variant<DDL_Float, DDL_String,DDL_Uint8,DDL_Uint32> metric;
		DDL_BufferType metricType;
		//properties
		ScratchPadString key;			
	};
	
	
}