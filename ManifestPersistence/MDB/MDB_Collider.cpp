#include "MDB_Collider.h"

using namespace Manifest_Persistence;

ForeignKey Manifest_Persistence::TableEntry(const DDL_Structure& structure, ColliderBuildTable& colliderBuildTable)
{
	MDB_Collider& entry = colliderBuildTable.entries.emplace_back();
	entry.colliderID = colliderBuildTable.nextTableIndex++;
	colliderBuildTable.mappedEntryKeys.insert({ structure.name.c_str(), entry.colliderID });

	const auto& collider{ HeapData<MDD_Collider>(structure) };
	entry.colliderType = collider.colliderGeometry;
	const auto colliderSize{ sizeof(MFfloat) * collider.colliderData.data.subBufferElements *
		collider.colliderData.data.subBufferCount };
	entry.colliderData = new MFfloat[colliderSize];
	memcpy(entry.colliderData, collider.colliderData.data.typeHeap, colliderSize);

	return entry.colliderID;
}