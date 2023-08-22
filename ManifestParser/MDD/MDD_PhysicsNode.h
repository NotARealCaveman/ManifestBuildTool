#pragma once

#include <ManifestParser/DDL_ExtendedTypes.h>

#include "MDD_Collider.h"
#include "MDD_RigidBody.h"

namespace Manifest_Parser
{
	static constexpr MFbool DYNAMIC{ true };
	static constexpr MFbool STATIC{ false };

	//stores the per physics object attributes of a body in the engine format -> later split into collider and rigid bodys in MDB/Binary
	//MDB/Binary physics nodes are to be repurposed to be an entire collection of the physics node properties
	struct MDD_PhysicsNode : public RegisteredBuilder
	{
		struct PropertyList {
			static constexpr int DYNAMIC{ 0 };
			static const std::map<std::string, uint32_t> typeProperties;
		};
		DDL_Structure* Build(const std::string_view& partitionedStructureView, DDL_ReferenceMap& referenceMap)final;
		//substructures
		MDD_Collider collider;
		MDD_RigidBodyParams rigidBodyParams;
		//nodes are static by default
		MFbool isDynamic{ false };
	};
}