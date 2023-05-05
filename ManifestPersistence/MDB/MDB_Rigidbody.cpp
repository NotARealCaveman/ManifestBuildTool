#include "MDB_Rigidbody.h"

using namespace Manifest_Persistence;

ForeignKey Manifest_Persistence::TableEntry(const DDL_Structure& structure, RigidbodyBuildTable& rigidbodyBuildTable)
{
	MDB_Rigidbody& entry = rigidbodyBuildTable.entries.emplace_back();
	entry.rigidBodyID = rigidbodyBuildTable.nextTableIndex++;
	rigidbodyBuildTable.mappedEntryKeys.insert({ structure.name.c_str(), entry.rigidBodyID });
	
	const auto& rigidBodyParams{ HeapData<MDD_RigidBodyParams>(structure) };
	entry.orientation = reinterpret_cast<const MFquaternion&>(rigidBodyParams.orientation.data.typeHeap);
	entry.position = reinterpret_cast<const MFpoint3&>(rigidBodyParams.position.data.typeHeap);
	entry.linearAccelaration = reinterpret_cast<const MFvec3&>(rigidBodyParams.linearAccelaration.data.typeHeap);
	entry.linearVelocity = reinterpret_cast<const MFvec3&>(rigidBodyParams.linearVelocity.data.typeHeap);
	entry.angularVelocity = reinterpret_cast<const MFvec3&>(rigidBodyParams.angularVelocity.data.typeHeap);
	entry.appliedForce = reinterpret_cast<const MFvec3&>(rigidBodyParams.appliedForce.data.typeHeap);
	entry.appliedTorque = reinterpret_cast<const MFvec3&>(rigidBodyParams.appliedTorque.data.typeHeap);
	entry.iMass = reinterpret_cast<const MFfloat&>(rigidBodyParams.iMass.data.typeHeap);
	entry.linearDamping = reinterpret_cast<const MFfloat&>(rigidBodyParams.linearDamping.data.typeHeap);
	entry.angularDamping = reinterpret_cast<const MFfloat&>(rigidBodyParams.angularDamping.data.typeHeap);

	return entry.rigidBodyID;
}