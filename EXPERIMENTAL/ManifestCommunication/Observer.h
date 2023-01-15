#pragma once
#include <vector>
#include <thread>

#include "EventSpace.h"

#include <EXPERIMENTAL/Typenames.h>
#include <ManifestGLUtility/DebugLogger.h>

using namespace Manifest_Experimental;

namespace Manifest_Communication
{
	struct UniqueObserverRegister
	{
		std::atomic<MFu64> registeredObservationTokens{ 0 };
		bool RegisterObserver(const ObservationToken& observationToken);
	};

	//Observer is an independent broker
	//the observer object is created with an observation token providing guaranteed, exclusive access to the message types of the event it is observing
	//template<typename ObservableSystem>
	struct Observer 
	{		
		//using ObservationToken = ObserverationToken<ObservableSystem>;
		//using EventMessage = EventMessage<ObservableSystem>;
		
		Observer(const ObservationToken& observationToken, UniqueObserverRegister& uniqueObserverRegister);
			
		
		const ObservationToken observationToken;
		//filled when observing the event space 
		//message processing is separate from event observation
		std::vector<EventMessage> observedEventMessages;
	};
}