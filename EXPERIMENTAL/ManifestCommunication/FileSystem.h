#pragma once
#include "Observer.h"

#include <ManifestGLUtility/DebugLogger.h>


namespace Manifest_Communication
{
	struct ObersvableFileSystem
	{
		enum class MessageTypes : MFu64
		{
			TYPE_MDB_GEOMETRYNODE = pow2(0),
			TYPE_MDB_GEOMETRYOBJECT = pow2(1),
			TYPE_MDB_MATERIAL = pow2(2),
		};		
	};
	using FileSystem= ObersvableFileSystem;
	using FileSystemMessageType = FileSystem::MessageTypes;
	using FileSystemObservationToken = ObservationToken;
	using FileSystemEvent = ObservableEvent;
	using FileSystemObserver = Observer;
	using FileSystemEventSpace = EventSpace;
}