#pragma once

#include <ManifestMathCOPY/Quaternion.h>
#include <ManifestMathCOPY/Point3.h>

#include <ManifestParser/MDD/MDD_Rigidbody.h>
#include <ManifestParser/DDL_ExtendedTypes.h>

#include "MDB_Table.h"

using namespace Manifest_Parser;
using namespace Manifest_Math;

namespace Manifest_Persistence
{
	struct MDB_Rigidbody
	{
		PrimaryKey mapID;
		MFquaternion orientation;
		MFpoint3 position;
		MFvec3 linearAccelaration;
		MFvec3 linearVelocity;
		MFvec3 angularVelocity;
		MFvec3 appliedForce;
		MFvec3 appliedTorque;
		MFfloat iMass;
		MFfloat linearDamping;
		MFfloat angularDamping;
	};
	using RigidbodyBuildTable = DatabaseBuildTable<MDB_Rigidbody>;
	ForeignKey TableEntry(const DDL_Structure& structure, RigidbodyBuildTable& rigidbodyBuildTable);
}