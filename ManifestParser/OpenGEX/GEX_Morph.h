#pragma once
#include <ManifestParser/RegisteredGenerators.h>

#include "GEX_Name.h"

namespace Manifest_Parser
{
	struct GEX_MorphWeight : public RegisteredBuilder
	{
		DDL_Structure*  Build(const std::string& partitionedSubstructure, DDL_ReferenceMap& referenceMap) final { return{}; } ;
		//substructures
		float morphWeight;
		//properties
		uint32_t index = 0;
	};

	struct GEX_Morph : public RegisteredBuilder
	{
		DDL_Structure* Build(const std::string& partitionedSubstructure, DDL_ReferenceMap& referenceMap) final { return{}; };
		//substructures
		GEX_Name name;
		//properties
		uint32_t index = 0;
		uint32_t* base;
	};
}	