#pragma once

#include <ManifestPersistence/Binary_Database.h>
#include <ManifestUtility/DebugLogger.h>
#include <EXPERIMENTAL/EXPERIMENTAL_RUNTIME_DATA_STRUCTURES.h>
#include "EventSpace.h"

using namespace Manifest_Persistence;
using namespace Manifest_Experimental;

namespace Manifest_Communication
{
	struct ObersvableFileSystem
	{		
		enum class MessageTypes : MFu64
		{
			MBD_GEOMETRYNODE = pow2(0),
			MBD_GEOMETRYOBJECT = pow2(1),
			MBD_MATERIAL = pow2(2),
			MBD_TEXTURE = pow2(3),
		};			
		void LoadMBD(const std::string& mbd, EventSpace& eventSpace);
	};

	using FileSystem = ObersvableFileSystem;
	using FileSystemMessageType = FileSystem::MessageTypes;
	using FileSystemObservationToken = ObservationToken;
	using FileSystemEvent = ObservableEvent;
	using FileSystemObserver = Observer;
	using FileSystemEventSpace = EventSpace;		
}