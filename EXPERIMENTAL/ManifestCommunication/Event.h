#pragma once
#include <vector>
#include <utility>

#include "Message.h"

#include <EXPERIMENTAL/Typenames.h>
#include <ManifestGLUtility/DebugLogger.h>

using namespace Manifest_Experimental;

namespace Manifest_Communication
{		
	//ObservableEvent is a snapshot of all the messages created during an event
	//When observing an event an observer may opt out of the event all together if any of the message types they are observing are not present 
	struct ObservableEvent
	{			
		ObservableEvent() = default;
		ObservableEvent(ObservableEvent&& other) noexcept;
		//encodes messages types present in event using operator|
		std::atomic<ObservationToken> eventToken;
		//holds each event message created during the event
		std::vector<Message> messages;
	};	
	using Event = ObservableEvent;
	//clears bits of event token with CAS
	void ClearObservedMessages(const ObservationToken& observationToken, std::atomic<ObservationToken>& eventToken);
}