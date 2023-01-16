#pragma once
#include <vector>
#include <thread>

#include "EventSpace.h"

#include <EXPERIMENTAL/Typenames.h>
#include <ManifestGLUtility/DebugLogger.h>

using namespace Manifest_Experimental;

namespace Manifest_Communication
{
	//exclusivity checked in debug mode via assert
	//Registration still must occur either way so returning the bool allows repurposing in relase mode to force application exit upon failure
	struct UniqueObserverRegister
	{
		std::atomic<MFu64> registeredObservationTokens{ 0 };
		bool RegisterObserver(const ObservationToken& observationToken);
	};

	//Observer is an independent broker
	//the observer object is created with an observation token providing guaranteed, exclusive access to the message types of the event it is observing	
	//the exclusivity is provided by UniqueObserverRegister
	struct Observer 
	{	
		Observer(const ObservationToken& observationToken, UniqueObserverRegister& uniqueObserverRegister);
		
		const ObservationToken observationToken;
		//filled when observing the event space
		std::vector<EventMessage> observedEventMessages;
	};
}