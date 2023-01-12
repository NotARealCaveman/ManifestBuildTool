#pragma once
#include <vector>

#include "Message.h"

#include <EXPERIMENTAL/Typenames.h>
#include <ManifestGLUtility/DebugLogger.h>

using namespace Manifest_Experimental;

namespace Manifest_Communication
{	
	//allows a compile time token to be created that allow access to specific event space messages
	template<typename ObservableSystem>
	using ObserverationToken = enum ObservableSystem::MessageTypes;	

	template<typename ObservableSystem>
	struct EventInformation
	{
		std::vector<enum ObservableSystem::MessageTypes> messageTypes;
		std::vector<Message> messages;
	};

	template<typename ObservableSystem>
	struct ObservableEvent
	{		
		ObserverationToken<ObservableSystem> eventToken;
		EventInformation<ObservableSystem> eventInformation;
	};	
}