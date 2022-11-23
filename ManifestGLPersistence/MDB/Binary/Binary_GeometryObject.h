#pragma once

#include <ManifestGLPersistence/MDB/Manifest_GeometryObject.h>

namespace Manifset_Persistence
{
	struct Binary_GeometryObject
	{
		struct Entry_Header
		{
			size_t	payloadSize;
			ForeignKey meshID;
			ForeignKey morphID;
		}header;
		void* payload;
	};
	//see Binary_Mesh.h for details
	size_t Convert_MDB(const MDB_GeometryObject& mesh, Binary_GeometryObject& binaryMesh);
}