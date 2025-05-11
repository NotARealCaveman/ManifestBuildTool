#pragma once
#include <vector>
#include <thread>

#include "Event.h"

#include <ManifestMemory/MemoryGuards/ExchangeLock.h>

using namespace Manifest_Memory;

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
			assert(!messages.empty(), && "ENSURE CALL TO HAS PENDING MESSAGES BEFORE PROCESSING EVENTS");
			ForwardFunction(function, messages, params...);
		}
		template<typename Function, typename... Params>
		void ProcessEvents_Unlocked(Function&& function, Params&&... params)
		{
			ForwardFunction(function, observedEventMessages, params...);
			observedEventMessages.clear();
		}
		inline const MFbool HasPendingMessages() { return !observedEventMessages.empty(); };
		const ObservationToken observationToken;		
		//Moves into messages - spins if moving for processing
		void ObserveEvent(Message&& messaoge);
	private:
		std::vector<Message> observedEventMessages;
		ExchangeLock messageLock;
	};
}