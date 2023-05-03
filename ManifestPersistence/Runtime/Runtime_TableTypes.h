#pragma once

#include <ManifestPhysics/State.h>
#include <ManifestTerrain/TerrainExtraction.h>
#include <ManifestCore/WorldConfiguration.h>

#include "Runtime_Table.h"


using namespace Manifest_Experimental;

namespace Manifest_Persistence
{	
	using NodeStateTable = Table<Manifest_Physics::StatePair, std::default_delete<Manifest_Physics::StatePair>>;
	using TerrainExtractionTable = Table<Manifest_Terrain::TerrainExtraction, Manifest_Terrain::TerrainExtractionDeleter>;
	using WorldConfigurationTable = Table<Manifest_Core::WorldConfiguration, WorldConfigurationDeleter>;
}