#pragma once
#include <vector>
#include <thread>

#include "Event.h"

#include <EXPERIMENTAL/Typenames.h>
#include <ManifestGLUtility/DebugLogger.h>

#include <EXPERIMENTAL/EXPERIMENTAL_RUNTIME_DATA_STRUCTURES.h>

using namespace Manifest_Experimental;

namespace Manifest_Communication
{
	class Observer;
	class EventSpace;

	//exclusivity checked in debug mode via assert
	//Registration still must occur either way so returning the bool allows repurposing in relase mode to force application exit upon failure
	class UniqueObserverRegister
	{
	private:
		friend EventSpace;

		ExchangeLock registrationLock;
		std::vector<Observer*> registeredObservers;		
	public:
		bool RegisterObserver(const Observer& observer);

		std::atomic<MFu64> registeredObservationTokens{ 0 };
	};

	//Observer is an independent broker
	//the observer object is created with an observation token providing guaranteed, exclusive access to the message types of the event it is observing	
	//the exclusivity is provided by UniqueObserverRegister
	class Observer
	{
	private:
		friend EventSpace;
		//allows meta messages to be generated
		using MessageProcessingFunction = Message*(*)(std::vector<Message>&);
		//Moves into messages - spins if moving for processing
		void ObserveEvent(Message&& message);

		std::vector<Message> observedEventMessages;
		ExchangeLock messageLock;
	public:
		Observer(const ObservationToken& observationToken, UniqueObserverRegister& uniqueObserverRegister);		
		//Processes messages - spins if writing for observation
		void ProcessEvents(const MessageProcessingFunction& processFunction);

		const ObservationToken observationToken;			
	};
}