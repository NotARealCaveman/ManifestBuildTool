#pragma once

#include <ManifestGLPersistence/MDB/Manifest_Mesh.h>

namespace Manifset_Persistence
{
	struct Binary_Mesh
	{
		struct Entry_Header
		{
			size_t payloadSize{ 0 };//stores vbo size in bytes
			size_t vboStride{ 0 };
			uint8_t activeArrayAttributes{ 0 };//8 bit code, bits 0-4 are p,uv,n,t,b 5-7 are unused
		}header;		
		float* payload;
	};
	//converts a manifest database format mesh into a binary mesh to be stored in the database file
	//returns the total size of the table entry for the binary table
	size_t Convert_MDB(const MDB_Mesh& mesh, const VertexTables& vertexTables, const IndexTable& indexTable, Binary_Mesh& binaryMesh);		
}