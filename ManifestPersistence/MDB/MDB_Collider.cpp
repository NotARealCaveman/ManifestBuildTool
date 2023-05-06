#include "MDB_Collider.h"

using namespace Manifest_Persistence;

ForeignKey Manifest_Persistence::TableEntry(const ScratchPadVector<DDL_Structure*>& colliderStructures, ColliderBuildTable& colliderBuildTable)
{
	MDB_Collider& entry = colliderBuildTable.entries.emplace_back();
	entry.colliderID = colliderBuildTable.nextTableIndex++;
	//colliderBuildTable.mappedEntryKeys.insert({ structure.name.c_str(), entry.colliderID });
	const auto colliderCount{ colliderStructures.size() };
	entry.colliderType = New<MFu8>(colliderCount);
	entry.colliderData = New<MFfloat*>(colliderCount);
	auto& colliderIndex{ entry.colliderCount };
	for (const auto& structure : colliderStructures)
	{		
		const auto& collider{ HeapData<MDD_Collider>(*structure) };
		entry.colliderType[colliderIndex] = collider.colliderGeometry;
		const auto colliderSize{ sizeof(MFfloat) * collider.colliderData.data.subBufferElements *
			collider.colliderData.data.subBufferCount };
		entry.colliderData[colliderIndex] = new MFfloat[colliderSize];
		memcpy(entry.colliderData[colliderIndex], collider.colliderData.data.typeHeap, colliderSize);
		++colliderIndex;
	}	
	return entry.colliderID;
}