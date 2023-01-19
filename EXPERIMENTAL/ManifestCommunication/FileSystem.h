#pragma once
#include "EventSpace.h"


#include <ManifestGLUtility/DebugLogger.h>

#include <ManifestGLPersistence/Binary_Database.h>
#include <ManifestGLPersistence/Runtime_Database.h>

using namespace Manifest_Persistence;

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

	ProcessMessage TEST_PROCESS_FUNC(std::vector<Message>& messages);

	using FileSystem = ObersvableFileSystem;
	using FileSystemMessageType = FileSystem::MessageTypes;
	using FileSystemObservationToken = ObservationToken;
	using FileSystemEvent = ObservableEvent;
	using FileSystemObserver = Observer;
	using FileSystemEventSpace = EventSpace;		
}