#pragma once
#include "EventSpace.h"

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
		static void OnLoad();		
		static void OnSave()=delete;
	};
	typedef ObersvableFileSystem FileSystem;
	typedef FileSystem::MessageTypes FileSystemMessageType;	
	typedef ObserverationToken<FileSystem> FileSystemToken;
	typedef ObservableEvent<FileSystem> FileSystemEvent;
	typedef Observer<FileSystem> FileSystemObserver;
	typedef EventSpace<FileSystem> FileSystemEventSpace;
}