#include "MDB_Terrain.h"

using namespace Manifest_Persistence;

ForeignKey Manifest_Persistence::TableEntry(const DDL_Structure& structure, TerrainBuildTable& terrainBuildTable)
{
	MDB_Terrain& entry = terrainBuildTable.entries.emplace_back();
	entry.terrainID = terrainBuildTable.nextTableIndex++;
	terrainBuildTable.mappedEntryKeys.insert({ structure.name.c_str(), entry.terrainID });

	for (const auto& substructure : structure.subSutructres)
	{
		switch (ExtractStructureType(substructure->identifier.c_str()))
		{
			case DDL_BufferTypes::DDL_int8:
			{
				const auto& SDFHeap{ HeapData<DDL_Int8>(*substructure) };
				entry.SDFSampleSize = SDFHeap.data.subBufferCount * SDFHeap.data.subBufferElements;
				const auto& samples{ reinterpret_cast<MFint8*>(SDFHeap.data.typeHeap) };				
				entry.terrainSDF = New<MFint8, 
					ScratchPad<MFint8>>(entry.SDFSampleSize);
				memcpy(entry.terrainSDF, samples, entry.SDFSampleSize);
				break;
			}
			case DDL_BufferTypes::DDL_uint8:
			{
				const auto& indexHeap{ HeapData<DDL_Uint8>(*substructure) };
				const auto& index{ *reinterpret_cast<MFu8*>(indexHeap.data.typeHeap) };
				entry.terrainID = index;
				break;
			}
			DEFAULT_BREAK
		};
	}
	return entry.terrainID;
}