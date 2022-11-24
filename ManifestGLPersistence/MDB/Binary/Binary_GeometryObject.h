#pragma once

#include <ManifestGLPersistence/MDB/Manifest_GeometryObject.h>

namespace Manifest_Persistence
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
	//see Buffer_Types.cpp for details
	size_t Convert_MDB(const MDB_GeometryObject& mesh, Binary_GeometryObject& binaryMesh);
}