#pragma once
#include <vector>
#include <thread>

#include <EXPERIMENTAL/Typenames.h>
#include <ManifestGLUtility/DebugLogger.h>

using namespace Manifest_Experimental;

namespace Manifest_Communication
{	
	//allows a compile time token to be created that allow access to specific event space messages;
	template<typename ObservableSystem>
	using ObserverationToken = enum ObservableSystem::MessageTypes;	

	template<typename ObservableSystem>
	struct Event
	{			
		std::vector<enum ObservableSystem::MessageTypes> messages;
	};

	template <typename ObservableSystem>
	struct EventSpace
	{

	};
}