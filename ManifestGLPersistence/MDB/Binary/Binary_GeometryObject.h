#pragma once

#include <ManifestGLPersistence/MDB/Manifest_GeometryObject.h>

namespace Manifset_Persistence
{
	struct Geometry_Indices
	{
		static constexpr uint8_t MESH_ID = 0;
		static constexpr uint8_t MORPH_ID = 1;
		static constexpr uint8_t TOTAL_IDS = MORPH_ID + 1;
	};

	struct Binary_GeometryObject
	{
		struct Entry_Header
		{
			size_t	payloadSize;
		}header;
		PrimaryKey* payload;
	};
	//see Binary_Mesh.h for details
	size_t Convert_MDB(const MDB_GeometryObject& mesh, Binary_GeometryObject& binaryMesh);
}