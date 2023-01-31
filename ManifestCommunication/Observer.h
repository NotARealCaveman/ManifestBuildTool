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

	//exclusivity checked in debug mode via assert
	//Registration still must occur either way so returning the bool allows repurposing in relase mode to force application exit upon failure
	class UniqueObserverRegister
	{
	private:
		friend EventSpace;

		ExchangeLock registrationLock;
		std::vector<Observer*> registeredObservers;		
	public:
		MFbool RegisterObserver(const Observer& observer);

		std::atomic<MFu64> registeredObservationTokens{ 0 };
	};
	//allows meta messages to be generated		
	using VOID_ = void*;
	template<typename T = VOID_,typename... Args>
	using MessageProcessingFunction = T*(*)(std::vector<Message>&, Args&...);

	//Observer is an independent broker
	//the observer object is created with an observation token providing guaranteed, exclusive access to the message types of the event it is observing	
	//the exclusivity is provided by UniqueObserverRegister
	class Observer
	{
	private:
		friend EventSpace;				
		//Moves into messages - spins if moving for processing
		void ObserveEvent(Message&& message);

		std::vector<Message> observedEventMessages;
		ExchangeLock messageLock;
	public:
		Observer(const ObservationToken& observationToken);		
		//Processes messages - spins if writing for observation	
		template<typename T = VOID, typename... Args>
		T* ProcessEvents(const MessageProcessingFunction<T,Args...> processFunction,Args&... args)
		{
			messageLock.Lock();
			std::vector<Message> messages = std::move(observedEventMessages);
			messageLock.Unlock();

			if (!messages.size())
				return nullptr;

			return processFunction(messages,args...);
		}
		const ObservationToken observationToken;			
	};
}