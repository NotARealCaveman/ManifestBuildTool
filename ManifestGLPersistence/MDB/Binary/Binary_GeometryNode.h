#pragma once

#include <ManifestGLPersistence/MDB/Manifest_GeometryNode.h>

namespace Manifset_Persistence
{	
	struct Binary_GeometryNode
	{
		struct Entry_Header
		{
			size_t	payloadSize{0};
			ForeignKey geometryID;
			ForeignKey materialID;
			
		}header;
		void* payload;//represents a transform if present
	};	
	size_t Convert_MDB(const MDB_GeometryNode& geometryNode,const ObjectRefTable& objectRefTable, const MaterialRefTable& materialRefTable, Binary_GeometryNode& binaryGeometryNode);
}