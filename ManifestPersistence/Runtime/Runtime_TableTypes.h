#pragma once

#include <EXPERIMENTAL/EXPERIMENTAL_RUNTIME_DATA_STRUCTURES.h>

#include "Runtime_Table.h"

using namespace Manifest_Experimental;

namespace Manifest_Persistence
{	
	using NodeStateTable = Table<Manifest_Experimental::StatePair_EXPERIMENTAL, std::default_delete< Manifest_Experimental::StatePair_EXPERIMENTAL>>;	
}