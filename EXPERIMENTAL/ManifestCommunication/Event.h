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
	using ObservationToken = MFu64;	
	
	using EventMessage = std::pair<ObservationToken, Message>;

	//ObservableEvent is a snapshot of all the messages created during an event
	//When observing an event an observer may opt out of the event all together if any of the message types they are observing are not present 
	struct ObservableEvent
	{			
		ObservableEvent() = default;
		ObservableEvent(ObservableEvent&& other) noexcept;
		//encodes messages types present in event using operator|
		ObservationToken eventToken;
		//holds each event message created during the event
		std::vector<EventMessage> messages;
	};	
	void ClearObservedMessages(const MFu64& messageType, ObservationToken& eventToken);
}