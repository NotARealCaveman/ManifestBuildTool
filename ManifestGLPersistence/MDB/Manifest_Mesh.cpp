#include "Manifest_Mesh.h"

using namespace Manifset_Persistence;

ForeignKey Manifset_Persistence::TableEntry(const DDL_Structure& structure, MeshTable& meshTable, VertexTables& vertexTables, IndexTable& indexTable)
{	
	MDB_Mesh& entry = meshTable.entries.emplace_back();
	entry.meshID = meshTable.nextTableIndex++; 
	meshTable.mappedEntryKeys.insert({structure.name, entry.meshID});	

	for (const auto& substructure : structure.subSutructres)
	{
		switch (ExtractStructureType(substructure.identifier))
		{
			case GEX_BufferTypes::GEX_VertexArray:
			{	
				auto& vaIDs = entry.vertexArrayIDs;
				auto bufferIndex{ HeapData<GEX_VertexArray>(substructure).bufferIndex };
				if (bufferIndex == 0)
					vaIDs.vertexID = TableEntry(substructure, vertexTables.vertexTable);
				else if (bufferIndex == 1)
					vaIDs.uvID = TableEntry(substructure, vertexTables.uvTable);
				else if (bufferIndex == 2)
					vaIDs.normalID = TableEntry(substructure, vertexTables.normalTable);
				else if (bufferIndex == 3)
					vaIDs.tangentID = TableEntry(substructure, vertexTables.tangentTable);
				else if (bufferIndex == 4)
					vaIDs.bitangentID = TableEntry(substructure, vertexTables.bitangentTable);
				break;
			}				
			case GEX_BufferTypes::GEX_IndexArray:
				entry.indexArrayID = TableEntry(substructure, indexTable);
				break;
			DEFAULT_BREAK
		}
	}
	return entry.meshID;
}