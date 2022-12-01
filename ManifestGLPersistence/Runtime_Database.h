#pragma once
#include <vector>	

#include "Binary_Database.h"

#include<EXPERIMENTAL/EXPERIMENTAL_RUNTIME_DATA_STRUCTURES.h>

namespace Manifest_Persistence
{
	typedef MFu32 GraphicID;

	struct Materials
	{
		Table<UniqueKey, GraphicID[3]> materailIDs;
	};

	struct GeometryNodes
	{		
		Table<UniqueKey, GraphicID> VAOs;
		Table<PrimaryKey, UniqueKey> materials;
	};		

	class ManifestRuntimeDatabase
	{
		public:								
			ManifestRuntimeDatabase(const ManifestBinaryDatabase& binaryDatabase, const WorldSpaces& worldSpaces, const GraphicResources& graphicResources);

			GeometryNodes geometryNodes;			
	};

	
}