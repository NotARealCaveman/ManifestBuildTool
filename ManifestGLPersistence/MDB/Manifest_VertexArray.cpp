#include "Manifest_VertexArray.h"

using namespace Manifest_Persistence;

ForeignKey Manifest_Persistence::TableEntry(const DDL_Structure& structure, VertexArrayTable& vertexArrayTable)
{	
	MDB_VertexArray& entry = vertexArrayTable.entries.emplace_back();
	entry.vertexArrayID = vertexArrayTable.nextTableIndex++;
	vertexArrayTable.mappedEntryKeys.insert({ structure.name,entry.vertexArrayID });	
	const DDL_Buffer& bufferData{ HeapData<GEX_VertexArray>(structure).vertexArray.data };
	entry.elements = bufferData.subBufferCount * bufferData.subBufferElements;
	entry.vertexData = new float[entry.elements];
	memcpy(entry.vertexData, bufferData.typeHeap, entry.elements * sizeof(float));
	return entry.vertexArrayID;
}