#pragma once

#include <ManifestGLPersistence/MDB/Manifest_GeometryNode.h>

namespace Manifset_Persistence
{	
	struct Binary_GeometryNode
	{
		struct Entry_Header
		{
			size_t	payloadSize;
			ForeignKey objectRefID;
			ForeignKey materialRefID;
			
		}header;
		void* payload;//transform if present
	};	
	size_t Convert_MDB(const MDB_GeometryNode& geometryNode, Binary_GeometryNode& binaryGeometryNode);
}