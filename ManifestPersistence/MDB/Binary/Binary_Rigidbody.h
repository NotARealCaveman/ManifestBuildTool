#pragma once

#include <ManifestPersistence/MDB/MDB_PhysicsNode.h>

namespace Manifest_Persistence
{
	struct Binary_RigidBody
	{
		struct Entry_Header
		{
			size_t	payloadSize{ 0 };			


		}header;
		MFfloat* payload;//represents a transform if present
	};
	//see Buffer_Types.cpp for details
	size_t Convert_MDB(const MDB_PhysicsNode& geometryNode, const ObjectRefBuildTable& objectRefBuildTable, const MaterialRefBuildTable& materialRefBuildTable, Binary_PhysicsNode& binaryPhysicsNode);
}