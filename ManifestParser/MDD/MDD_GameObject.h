#pragma once

#include <ManifestDefinitions/GameObject_Definitions.h>
#include <ManifestParser/OpenDDL/DDL_Primitives.h>
#include <ManifestParser/OpenDDL/DDL_Reference.h>
#include <ManifestParser/OpenGEX/GEX_GeometryObject.h>

#include "MDD_PhysicsNode.h"



namespace Manifest_Parser
{	
	struct MDD_GameObject : public RegisteredBuilder
	{
		DDL_Structure* Build(const std::string_view& partitionedStructureView, DDL_ReferenceMap& referenceMap) final;
		//substructures
		//references are paired with each active bit in the type
		//ex index 1 = 2nd lowest active bit
		DDL_Uint64 OBJECT_TYPE;	 
		DDL_Reference objectReferences;
	};
}