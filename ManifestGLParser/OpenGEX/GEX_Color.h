#pragma once
#include <string>

#include <ManifestGLParser/RegisteredGenerators.h>
#include <ManifestGLParser/OpenDDL/DDL_Primitives.h>

#include "GEX_Buffers.h"

namespace Manifest_Parser
{
	enum class GEX_ColorType
	{
		RGB, RGBA
	};

	struct GEX_Color : public RegisteredBuilder
	{
		struct PropertyList {
			static constexpr int ATTRIB{ 0 };
			static const std::map<std::string, uint32_t> typeProperties;
		};
		DDL_Structure* Build(const std::string& partitionedStructure, DDL_ReferenceMap& referenceMap)  final;

		//substructures
		GEX_ColorType colorType;
		DDL_Float channel;//[4]
		//properties
		ScratchPadString attrib;
	};
}