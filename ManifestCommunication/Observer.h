#pragma once
#include <vector>
#include <thread>

#include "Event.h"

#include <ManifestUtility/Typenames.h>
#include <ManifestMemory/MemoryGuards/ExchangeLock.h>

using namespace Manifest_Memory;
using namespace Manifest_Utility;

namespace Manifest_Communication
{
	class Observer;
	class EventSpace;
		
	class UniqueObserverRegister
	{
	private:
		friend EventSpace;
				
		std::vector<Observer*> registeredObservers;		
	public:
		MFbool RegisterObserver(const Observer& observer);

		MFu64 registeredObservationTokens{ 0 };
	};

	//Observer is an independent broker
	//the observer object is created with an observation token providing guaranteed, exclusive access to the message types of the event it is observing	
	//the exclusivity is provided by UniqueObserverRegister
	class Observer
	{	
	public:
		Observer(const ObservationToken& observationToken);	
		//Processes messages - spins if writing for observation	
		template<typename Function, typename... Params>
		void ProcessEvents(Function&& function, Params&&... params)
		{			
			messageLock.Lock();
			std::vector<Message> messages = std::move(observedEventMessages);			
			messageLock.Unlock();

			//messages are cleaned up upon exiting function
			if (!messages.empty())							
				std::invoke(function, messages, params...);
		}
		inline const MFbool HasPendingMessages() { return !observedEventMessages.empty(); };
		const ObservationToken observationToken;		
	private:
		friend EventSpace;				
		//Moves into messages - spins if moving for processing
		void ObserveEvent(Message&& messaoge);

		std::vector<Message> observedEventMessages;
		ExchangeLock messageLock;
	};
}