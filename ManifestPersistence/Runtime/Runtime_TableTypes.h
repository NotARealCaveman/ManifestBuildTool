#pragma once

#include <ManifestPhysics/State.h>
#include <ManifestTerrain/VoxelStructs.h>

#include "Runtime_Table.h"


using namespace Manifest_Experimental;

namespace Manifest_Persistence
{	
	using NodeStateTable = Table<Manifest_Experimental::StatePair_EXPERIMENTAL, std::default_delete< Manifest_Experimental::StatePair_EXPERIMENTAL>>;
	using VoxelBlockTable = Table<Manifest_Terrain::VoxelBlock, std::default_delete<Manifest_Terrain::VoxelBlock>>;
}