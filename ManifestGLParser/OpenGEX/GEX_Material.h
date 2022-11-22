#pragma once
#include <vector>

#include <ManifestGLParser/OpenDDL/DDL_Reference.h>

#include "GEX_Name.h"
#include "GEX_Param.h"
#include "GEX_Texture.h"

namespace Manifest_Parser
{
	struct GEX_Material : public RegisteredBuilder
	{
		struct PropertyList {
			static constexpr int TWOSIDED{ 0 };			
			static const std::map<std::string, uint32_t> typeProperties;
		};
		DDL_Structure Build(const std::string& partitionedStructure, DDL_ReferenceMap& referenceMap) final;

		//substructures
		GEX_Name name;
		std::vector<GEX_Color> colors;
		std::vector<GEX_Param> parameters;
		std::vector<GEX_Spectrum> spectrums;
		std::vector<GEX_Texture> textures;
		//properties
		bool two_sided = false;
	};

	struct GEX_MaterialRef : public DDL_Reference
	{		
		struct PropertyList {
			static constexpr int INDEX{ 0 };			
			static const std::map<std::string, uint32_t> typeProperties;		
		};
		DDL_Structure Build(const std::string& partitionedStructure, DDL_ReferenceMap& referenceMap) final;

		//substructures
		GEX_Material* ref;
		//properties
		uint32_t index = 0;
	};
}