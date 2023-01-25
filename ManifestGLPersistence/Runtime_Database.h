#pragma once
#include <chrono>
#include <vector>	
#include <functional>
#include <string>

#include "Binary_Database.h"
#include "Runtime_Table.h"

#include <EXPERIMENTAL/ManifestCommunication/FileSystem.h>
#include <EXPERIMENTAL/EXPERIMENTAL_RUNTIME_DATA_STRUCTURES.h>
#include <EXPERIMENTAL/RNG.h>
#include <EXPERIMENTAL/Manifest_Allocator.h>

using namespace Manifest_Memory;
using namespace Manifest_Experimental;
using namespace Manifest_Communication;

namespace Manifest_Persistence
{
	//Currently exploring a push/pull paradigm for updating and centralizing shared game state in the runtime database
	class ManifestRuntimeDatabase
	{
	};	

	void SimThread(ManifestRuntimeDatabase& runtimeDatabase);
	void RenderThread(ManifestRuntimeDatabase& runtimeDatabase);
	void MessageThread();	
}

