#include "Manifest_Material.h"

using namespace Manifest_Persistence;

ForeignKey Manifest_Persistence::TableEntry(const DDL_Structure& structure, MaterialTable& materialTable, ColorTable& colorTable)
{	
	MDB_Material& entry = materialTable.entries.emplace_back();
	entry.materialID = materialTable.nextTableIndex++; 
	materialTable.mappedEntryKeys.insert({ structure.name,entry.materialID });
	for (const auto& substructure : structure.subSutructres)
	{
		switch (ExtractStructureType(substructure.identifier))
		{
			case GEX_BufferTypes::GEX_Color://tbd
				entry.materialCode = TableEntry(substructure, colorTable);
				break;
			DEFAULT_BREAK
		}
	}
	return entry.materialID;
}