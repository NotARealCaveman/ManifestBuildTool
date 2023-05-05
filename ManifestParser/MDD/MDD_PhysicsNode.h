#pragma once

#include <ManifestParser/DDL_ExtendedTypes.h>
#include <ManifestParser/OpenDDL/DDL_Reference.h>

#include "MDD_Collider.h"
#include "MDD_RigidBody.h"

namespace Manifest_Parser
{
	struct MDD_PhysicsNode : public RegisteredBuilder
	{
		DDL_Structure* Build(const std::string_view& partitionedStructureView, DDL_ReferenceMap& referenceMap)final;
		//substructures
		MDD_Collider collider;
		MDD_RigidBodyParams rigidBodyParams;
	};
}