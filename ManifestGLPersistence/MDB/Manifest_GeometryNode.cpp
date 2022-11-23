#include "Manifest_GeometryNode.h"

using namespace Manifset_Persistence;

ForeignKey Manifset_Persistence::TableEntry(const DDL_Structure& structure, const GeometryObjectTable& geometryObjectTable, ObjectRefTable& objectRefTable)
{
	MDB_ObjectRef& entry = objectRefTable.entries.emplace_back();
	entry.objectRefID = objectRefTable.nextTableIndex++;
	objectRefTable.mappedEntryKeys.insert({ structure.name,entry.objectRefID });
	const GEX_ObjectRef& ref{ HeapData<GEX_ObjectRef>(structure) };
	entry.numReferences = ref.referenceNames.size();
	entry.geometryIDs = new ForeignKey[entry.numReferences];	
	for (auto objectIndex = 0; objectIndex < entry.numReferences; ++objectIndex)
	{
		auto objectRef = geometryObjectTable.mappedEntryKeys.find(ref.referenceNames[objectIndex]);
		if (objectRef != geometryObjectTable.mappedEntryKeys.end())
			entry.geometryIDs[objectIndex] = objectRef->second;
		else
			entry.geometryIDs[objectIndex] = KEY_NOT_PRESENT;
	}

	return entry.objectRefID;
}

ForeignKey Manifset_Persistence::TableEntry(const DDL_Structure& structure, const MaterialTable& materialTable, MaterialRefTable& materialRefTable)
{	
	MDB_MaterialRef& entry = materialRefTable.entries.emplace_back();
	entry.materialRefID = materialRefTable.nextTableIndex++;
	materialRefTable.mappedEntryKeys.insert({ structure.name,entry.materialRefID });
	const GEX_MaterialRef& ref{ HeapData<GEX_MaterialRef>(structure) };
	entry.numReferences = ref.referenceNames.size();
	entry.materialIDs = new ForeignKey[entry.numReferences];
	for (auto objectIndex = 0; objectIndex < entry.numReferences; ++objectIndex)
	{
		auto objectRef = materialTable.mappedEntryKeys.find(ref.referenceNames[objectIndex]);
		if (objectRef != materialTable.mappedEntryKeys.end())
			*(entry.materialIDs + objectIndex) = objectRef->second;
		else
			*(entry.materialIDs + objectIndex) = KEY_NOT_PRESENT;
	}

	return entry.materialRefID;
}
ForeignKey Manifset_Persistence::TableEntry(const DDL_Structure& structure, const GeometryObjectTable& geometryObjectTable, const MaterialTable& materialTable, GeometryNodeTable& geometryNodeTable, ObjectRefTable& objectRefTable, MaterialRefTable& materialRefTable)
{
	MDB_GeometryNode& entry = geometryNodeTable.entries.emplace_back();
	entry.nodeID = geometryNodeTable.nextTableIndex++;
	geometryNodeTable.mappedEntryKeys.insert({ structure.name, entry.nodeID });
	
	for (const auto& substructure : structure.subSutructres)
	{
		switch (ExtractStructureType(substructure.identifier))
		{
			case GEX_BufferTypes::GEX_ObjectRef:
				entry.objectRefID = TableEntry(substructure, geometryObjectTable, objectRefTable);
				break;
			case GEX_BufferTypes::GEX_MaterialRef:
				entry.materialRefID = TableEntry(substructure, materialTable, materialRefTable);
				break;
			case GEX_BufferTypes::GEX_Transform:	
			{
				entry.transform = new MDB_Transform;				
				auto transform{ HeapData<GEX_Transform>(substructure).field.data.typeHeap };
				memcpy(entry.transform->field, transform, sizeof(float) * 16);
				break;
			}
			DEFAULT_BREAK
		}
	}

	return entry.nodeID;
}
