#pragma once
#include <chrono>
#include <vector>	
#include <functional>
#include <string>

#include <ManifestCommunication/FileSystem.h>
#include <ManifestMemory/Manifest_Allocator.h>
#include <EXPERIMENTAL/EXPERIMENTAL_RUNTIME_DATA_STRUCTURES.h>

#include <ManifestPersistence/Runtime/Runtime_TableTypes.h>
#include "Binary_Database.h"

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

