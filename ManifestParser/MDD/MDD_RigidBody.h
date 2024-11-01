#pragma once
#include <map>
#include <string>

#include <ManifestParser/OpenDDL/DDL_Primitives.h>

namespace Manifest_Parser
{
	namespace RIGID_BODY_PARAMS
	{
		static constexpr MFu8 ORIENTATION{ 1 };
		static constexpr MFu8 POSITION{ ORIENTATION+1 };
		static constexpr MFu8 ACCELARATION{ POSITION+1 };
		static constexpr MFu8 LINEAR_VELOCITY{ ACCELARATION+1 };
		static constexpr MFu8 ANGULAR_VELOCITY{ LINEAR_VELOCITY+1 };		
		static constexpr MFu8 BODY_SCALE{ ANGULAR_VELOCITY + 1 };
		static constexpr MFu8 APPLIED_FORCE{ BODY_SCALE +1 };
		static constexpr MFu8 APPLIED_TORQUE{ APPLIED_FORCE+1 };
		static constexpr MFu8 IMASS{ APPLIED_TORQUE+1 };
		static constexpr MFu8 LINEAR_DAMPING{ IMASS+1 };
		static constexpr MFu8 ANGULAR_DAMPING{ LINEAR_DAMPING + 1 };
	}

	static const std::map<std::string, MFu8> rigidBodyParamList
	{
		{"orientation",RIGID_BODY_PARAMS::ORIENTATION},
		{"position",RIGID_BODY_PARAMS::POSITION},		
		{"linearAccelaration",RIGID_BODY_PARAMS::ACCELARATION},
		{"linearVelocity",RIGID_BODY_PARAMS::LINEAR_VELOCITY},
		{"angularVelocity",RIGID_BODY_PARAMS::ANGULAR_VELOCITY},
		{"scale",RIGID_BODY_PARAMS::BODY_SCALE},
		{"appliedForce",RIGID_BODY_PARAMS::APPLIED_FORCE},
		{"appliedTorque",RIGID_BODY_PARAMS::APPLIED_TORQUE},
		{"iMass",RIGID_BODY_PARAMS::IMASS},
		{"linearDamping",RIGID_BODY_PARAMS::LINEAR_DAMPING},
		{"angularDamping",RIGID_BODY_PARAMS::ANGULAR_DAMPING}
	};
	

	struct MDD_RigidBodyParams : public RegisteredBuilder
	{
		DDL_Structure* Build(const std::string_view& partitionedStructureView, DDL_ReferenceMap& referenceMap)final;
		//substructures
		DDL_Float orientation;
		DDL_Float position;
		DDL_Float linearAccelaration;
		DDL_Float linearVelocity;
		DDL_Float angularVelocity;
		DDL_Float scale;
		DDL_Float appliedForce;
		DDL_Float appliedTorque;
		DDL_Float iMass;
		DDL_Float linearDamping;
		DDL_Float angularDamping;		
	};
}