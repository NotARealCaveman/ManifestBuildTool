#pragma once
#include <map>
#include <string>

#include <ManifestParser/OpenDDL/DDL_Primitives.h>

namespace Manifest_Parser
{
	struct RigidBodyParams : public RegisteredBuilder
	{
		DDL_Structure* Build(const std::string_view& partitionedStructureView, DDL_ReferenceMap& referenceMap)final;
		//substructures
		
	};
}