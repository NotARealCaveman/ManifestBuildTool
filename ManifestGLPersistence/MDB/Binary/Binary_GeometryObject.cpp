#include "Binary_GeometryObject.h"

using namespace Manifset_Persistence;

size_t Manifset_Persistence::Convert_MDB(const MDB_GeometryObject& geometryObject, Binary_GeometryObject& binaryGeometryObject)
{		
	binaryGeometryObject.header.payloadSize = sizeof(PrimaryKey) * Geometry_Indices::TOTAL_IDS;
	binaryGeometryObject.payload = new PrimaryKey[Geometry_Indices::TOTAL_IDS];
	binaryGeometryObject.payload[Geometry_Indices::MESH_ID] = geometryObject.meshID;
	binaryGeometryObject.payload[Geometry_Indices::MORPH_ID] = geometryObject.morphID;

	return EntrySize(binaryGeometryObject);
}