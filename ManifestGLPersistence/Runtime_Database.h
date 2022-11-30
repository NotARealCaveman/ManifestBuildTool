#pragma once
#include <vector>	

#include "Binary_Database.h"

#include<EXPERIMENTAL/EXPERIMENTAL_RUNTIME_DATA_STRUCTURES.h>

namespace Manifest_Persistence
{

	struct GeometryNodes
	{		
		Table<UniqueKey, MFu32> VAOs;		
	};		

	class ManifestRuntimeDatabase
	{
		public:								
			ManifestRuntimeDatabase(const ManifestBinaryDatabase& binaryDatabase, const WorldSpaces& worldSpaces, const GraphicResources& graphicResources);

			GeometryNodes geometryNodes;
	};

	
}