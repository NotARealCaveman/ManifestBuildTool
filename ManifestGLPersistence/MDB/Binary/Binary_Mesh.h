#pragma once

#include <ManifestGLPersistence/MDB/Manifest_Mesh.h>

namespace Manifest_Persistence
{
	struct Binary_Mesh
	{
		struct Entry_Header
		{
			size_t payloadSize{ 0 };//stores vbo+ebo size in bytes
			size_t eboOffset{ 0 };//stores the byte offset from the beginning of the payload to the index data. interleaved float data is stored immediately followed by the integer index data
			size_t vboStride{ 0 };
			uint8_t activeArrayAttributes{ 0 };//8 bit code, bits 0-4 are p,uv,n,t,b 5-7 are unused
		}header;		
		float* payload;
	};
	//see Buffer_Types.cpp for details
	size_t Convert_MDB(const MDB_Mesh& mesh, const VertexTables& vertexTables, const IndexTable& indexTable, Binary_Mesh& binaryMesh);		
}