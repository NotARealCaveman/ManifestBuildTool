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
		EventInformation() = default;
		EventInformation(EventInformation&& other)
			 : messageTypes{other.messageTypes}
		{
			messages.reserve(other.messages.size());
			std::for_each(other.messages.begin(), other.messages.end(), [&](Message& message) {messages.emplace_back(std::move(message)); });		
		}
		std::vector<enum ObservableSystem::MessageTypes> messageTypes;
		std::vector<Message> messages;
	};

	template<typename ObservableSystem>
	struct ObservableEvent
	{			
		ObservableEvent() {};
		ObservableEvent(ObservableEvent&& other)
			 : eventToken{other.eventToken}, eventInformation{std::move(other.eventInformation)} {}
		ObserverationToken<ObservableSystem> eventToken;
		EventInformation<ObservableSystem> eventInformation;
	};	
}