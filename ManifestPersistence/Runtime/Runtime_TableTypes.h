#pragma once

#include <ManifestPhysics/SimulationState.h>
#include <ManifestPhysics/IntelligenceEngine/ActionPlanner.h>
#include <ManifestTerrain/TerrainExtraction.h>
#include <ManifestCore/WorldConfiguration.h>

#include "Runtime_Table.h"


using namespace Manifest_Experimental;

namespace Manifest_Persistence
{	
	using SimulationStateTable = Table<Manifest_Simulation::SimulationState, std::default_delete<Manifest_Simulation::SimulationState>>;
	using TerrainExtractionTable = Table<Manifest_Terrain::TerrainExtraction, Manifest_Terrain::TerrainExtractionDeleter>;
	using WorldConfigurationTable = Table<Manifest_Core::WorldConfiguration, Manifest_Core::WorldConfigurationDeleter>;
	using ActionPlanTable = Table<const Manifest_Simulation::ActionPlan, Manifest_Simulation::ActionPlanDeleter>;
}