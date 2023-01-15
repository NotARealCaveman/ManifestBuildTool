#pragma once
#include <vector>
#include <utility>

#include "Message.h"

#include <EXPERIMENTAL/Typenames.h>
#include <ManifestGLUtility/DebugLogger.h>

using namespace Manifest_Experimental;

namespace Manifest_Communication
{	
	//token that allow access to specific event space messages
	template<typename ObservableSystem>
	using ObserverationToken = enum ObservableSystem::MessageTypes;		
	
	template<typename ObservableSystem>
	using EventMessage = std::pair<ObserverationToken<ObservableSystem>, Message>;

	//ObservableEvent is a snapshot of all the messages created during an event
	//When observing an event an observer may opt out of the event all together if any of the message types they are observing are not present 
	template<typename ObservableSystem>
	struct ObservableEvent
	{			
		using EventMessage = EventMessage<ObservableSystem>;

		ObservableEvent() {};
		ObservableEvent(ObservableEvent&& other)
			: eventToken{ other.eventToken }, messages{ std::move(other.messages) } {};
		//encodes messages types present in event using operator|
		ObserverationToken<ObservableSystem> eventToken;	
		//holds each event message created during the event
		std::vector<EventMessage> messages;
	};	
}