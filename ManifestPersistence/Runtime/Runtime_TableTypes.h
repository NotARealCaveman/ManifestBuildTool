#pragma once

#include <ManifestSimulation/SimulationState.h>
#include <ManifestSimulation/IntelligenceEngine/ActionPlanner.h>
#include <ManifestTerrain/TerrainExtraction.h>
#include <ManifestCore/WorldConfiguration.h>

#include "Runtime_Table.h"


using namespace Manifest_Experimental;

namespace Manifest_Persistence
{	
	using SimulationStateTable = Table<Manifest_Simulation::SimulationState, std::default_delete<Manifest_Simulation::SimulationState>>;
	using TerrainDataTable = Table<Manifest_Terrain::TerrainData, std::default_delete<Manifest_Terrain::TerrainData>>;
	using WorldConfigurationTable = Table<Manifest_Core::WorldConfiguration, Manifest_Core::WorldConfigurationDeleter>;
	using ActionPlanTable = Table<Manifest_Simulation::ActionPlan, std::default_delete<Manifest_Simulation::ActionPlan>>;
}