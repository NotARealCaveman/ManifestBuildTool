#pragma once

#include <ManifestGLPersistence/Manifest_Table.h>

#include "Manifest_Material.h"
#include "Manifest_GeometryObject.h"

using namespace Manifest_Parser;

namespace Manifest_Persistence
{
	struct MDB_ObjectRef
	{
		PrimaryKey objectRefID = KEY_NOT_PRESENT;
		ForeignKey* geometryIDs = BUFFER_NOT_PRESENT;
		size_t numReferences = 0;
	};
	typedef DatabaseTable<MDB_ObjectRef> ObjectRefTable;
	ForeignKey TableEntry(const DDL_Structure& structure,const GeometryObjectTable& geometryObjectTable, ObjectRefTable& objectRefTable);

	struct MDB_MaterialRef
	{
		PrimaryKey materialRefID = KEY_NOT_PRESENT;
		ForeignKey* materialIDs = BUFFER_NOT_PRESENT;
		size_t numReferences = 0;
	};
	typedef DatabaseTable<MDB_MaterialRef> MaterialRefTable;
	ForeignKey TableEntry(const DDL_Structure& structure, const MaterialTable& materialTable, MaterialRefTable& materialRefTable);

	
	
	//every transform will end up being 16 characters - this acts as a conversion type between gex transforms to mdb transforms
	struct MDB_Transform { float field[TransformSize]; };

	struct MDB_GeometryNode
	{
		PrimaryKey nodeID = KEY_NOT_PRESENT;
		ForeignKey objectRefID = KEY_NOT_PRESENT;
		ForeignKey materialRefID = KEY_NOT_PRESENT;
		MDB_Transform* transform = BUFFER_NOT_PRESENT;
		//const uint32_t morphWeight;
	};
	typedef DatabaseTable<MDB_GeometryNode> GeometryNodeTable;
	ForeignKey TableEntry(const DDL_Structure& structure,const GeometryObjectTable& geometryObjectTable, const MaterialTable& materialTable, GeometryNodeTable& geometryNodeTable, ObjectRefTable& objectRefTable, MaterialRefTable& materialRefTable);
}