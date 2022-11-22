#include "Manifest_IndexArray.h"

using namespace Manifset_Persistence;

ForeignKey Manifset_Persistence::TableEntry(const DDL_Structure& structure, IndexTable& indexTable)
{	
	MDB_IndexArray& entry = indexTable.entries.emplace_back();
	entry.indexArrayID = indexTable.nextTableIndex++; 
	indexTable.mappedEntryKeys.insert({ structure.name,entry.indexArrayID });
	const DDL_Buffer& bufferData{ HeapData<GEX_VertexArray>(structure).vertexArray.data };
	entry.elements = bufferData.subBufferCount * bufferData.subBufferElements;
	entry.indexData = new uint32_t[entry.elements];
	memcpy(entry.indexData, bufferData.typeHeap, entry.elements * sizeof(uint32_t));
	return entry.indexArrayID;
}

