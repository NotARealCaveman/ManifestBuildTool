#pragma once

#include <ManifestParser/DDL_ExtendedTypes.h>
#include <ManifestParser/DDL_ExtendedTypes.h>

#include "MDB_PhysicsNode.h"
#include "MDB_GeometryNode.h"

#include "MDB_Table.h"

namespace Manifest_Persistence
{	
	//to be moved to own file
	struct MDB_Ref
	{
		PrimaryKey refID = KEY_NOT_PRESENT;
		ForeignKey* referencesBuffer = BUFFER_NOT_PRESENT;
		size_t numReferences = 0;
	};
	typedef DatabaseBuildTable<MDB_Ref> RefBuildTable;
	template<typename MDB_Type, typename Ref_Type>
	ForeignKey TableEntry(const DDL_Structure& structure, const DatabaseBuildTable<MDB_Type>& databaseBuildTable, RefBuildTable& refBuildTable)
	{
		MDB_Ref& entry = refBuildTable.entries.emplace_back();
		entry.refID = refBuildTable.nextTableIndex++;
		refBuildTable.mappedEntryKeys.insert({ structure.name.c_str(),entry.refID });
		const Ref_Type& ref{ HeapData<Ref_Type>(structure) };
		entry.numReferences = ref.referenceNames.size();		
		entry.referencesBuffer = New<ForeignKey, ScratchPad<ForeignKey>>(entry.numReferences);
		for (auto objectIndex = 0; objectIndex < entry.numReferences; ++objectIndex)
		{
			auto dataRef = geometryObjectBuildTable.mappedEntryKeys.find(ScratchPadString{ static_cast<std::string>(ref.referenceNames[objectIndex]) });
			if (dataRef != databaseBuildTable.mappedEntryKeys.end())
				entry.referencesBuffer[objectIndex] = dataRef->second;
			else
				entry.referencesBuffer[objectIndex] = KEY_NOT_PRESENT;
		}

		return entry.refID;
	}

	struct MDB_RigidBodyObject
	{
		PrimaryKey rigidBodyObjectID;
		ForeignKey physicsNodeRefID;
		ForeignKey graphicsNodeRefID;
	};
	using RigidBodyObjectBuildTable = DatabaseBuildTable<MDB_RigidBodyObject>;
	ForeignKey TableEntry(const DDL_Structure& structure, RigidBodyObjectBuildTable& rigidBodyObjectBuildTable, PhysicsNodeBuildTable& physicsNodeBuildTable, GeometryNodeBuildTable& geometryNodeBuildTable);
}