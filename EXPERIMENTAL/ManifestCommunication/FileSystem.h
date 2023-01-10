#pragma once
#include "EventSystem.h"

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
		static void OnLoad(const int& loadEvent);		
	};

	typedef ObersvableFileSystem FileSystem;
	typedef FileSystem::MessageTypes FileSystemMessageType;	
	typedef
		ObservableGroupTriggers<FileSystem> FileSystemTriggers;
	typedef EventSpace<FileSystem> FileSystemEvents;
	typedef ObserverationToken<FileSystem> FileSystemToken;
}