#pragma once
#include <vector>
#include <thread>

#include "Event.h"

#include <EXPERIMENTAL/Typenames.h>
#include <ManifestGLUtility/DebugLogger.h>

using namespace Manifest_Experimental;

namespace Manifest_Communication
{
	//structure that ill be used to assert runtime uniqueness of registered observers
	struct UniqueObserver
	{

	};
	//Observer is an independent broker
	//the observer object is created with an observation token providing guaranteed, exclusive access to the message types of the event it is observing
	template<typename ObservableSystem>
	struct Observer 
	{		
		using ObservationToken = ObserverationToken<ObservableSystem>;
		using EventMessage = EventMessage<ObservableSystem>;
		
		Observer(const ObservationToken& token)
			:observationToken{ token } {}
		
		const ObservationToken observationToken;
		//filled when observing the event space 
		//message processing is separate from event observation
		std::vector<EventMessage> observedEventMessages;
	};
}