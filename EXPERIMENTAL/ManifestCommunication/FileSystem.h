#pragma once
#include "Triggers.h"
#include "MessageOperators.h"

#include <ManifestGLUtility/DebugLogger.h>


namespace Manifest_Communication
{
	struct ObersvableFileSystem
	{
		enum class MessageTypes : int
		{
			TYPE_MDB_GEOMETRYNODE = pow2(0),
			TYPE_MDB_GEOMETRYOBJECT = pow2(1),
			TYPE_MDB_MATERIAL = pow2(2),
		};
		static void OnLoad(const int& loadEvent);
	};
	typedef
		ObservableGroupTriggers<ObersvableFileSystem> FileSystemTriggers;	
	typedef ObersvableFileSystem::MessageTypes FileSystemMessageType;
}