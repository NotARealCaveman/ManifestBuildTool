#include "Manifest_GeometryObject.h"

using namespace Manifset_Persistence;

ForeignKey Manifset_Persistence::TableEntry(const DDL_Structure& structure, GeometryObjectTable& geometryObjectTable, MeshTable& meshTable, VertexTables& vertexTables, IndexTable& indexTable)
{		
	MDB_GeometryObject& entry = geometryObjectTable.entries.emplace_back();
	entry.geometryID = geometryObjectTable.nextTableIndex++;
	geometryObjectTable.mappedEntryKeys.insert({ structure.name,entry.geometryID });
	entry.geometryID = geometryObjectTable.nextTableIndex++;
	for (const auto& substructure : structure.subSutructres)
	{
		switch (ExtractStructureType(substructure.identifier))
		{
			case GEX_BufferTypes::GEX_Mesh:
				entry.meshID = TableEntry(substructure, meshTable, vertexTables, indexTable);
				break;
			DEFAULT_BREAK
		}
	}	
	return entry.geometryID;
}