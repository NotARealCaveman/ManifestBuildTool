#pragma once

#include <ManifestPhysics/State.h>
#include <ManifestTerrain/TerrainFlags.h>

#include "Runtime_Table.h"


using namespace Manifest_Experimental;

namespace Manifest_Persistence
{	
	using NodeStateTable = Table<Manifest_Experimental::StatePair_EXPERIMENTAL, std::default_delete< Manifest_Experimental::StatePair_EXPERIMENTAL>>;	
	using TerrainNodeTable = Table<Manifest_Terrain::TerrainNode, std::default_delete< Manifest_Terrain::TerrainNode>>;
}