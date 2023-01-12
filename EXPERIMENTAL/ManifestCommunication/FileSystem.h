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
		static void OnLoad();		
		static void OnSave()=delete;
	};
	typedef ObersvableFileSystem FileSystem;
	typedef FileSystem::MessageTypes FileSystemMessageType;	
	typedef
		ObservableGroupTriggers<FileSystem> FileSystemTriggers;	
	typedef ObserverationToken<FileSystem> FileSystemToken;
	typedef ObservableEvent<FileSystem> FileSystemEvent;


	struct FSObserver
	{
		const FileSystemToken observerToken;

	};
}