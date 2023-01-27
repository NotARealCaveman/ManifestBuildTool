﻿#include "Observer.h"

using namespace Manifest_Communication;
///UNIQUE OBSERVER REGISTER
bool UniqueObserverRegister::RegisterObserver(const Observer& observer)
{
	const auto& observationToken = observer.observationToken;
	auto expected = registeredObservationTokens.load(std::memory_order_acquire);
	if (expected & observationToken)
		return false;
	auto desired = expected | observationToken;
	while (!registeredObservationTokens.compare_exchange_strong(expected, desired, std::memory_order_release, std::memory_order_relaxed))
	{		
		if (expected & observationToken)
			return false;//someone else registered... erroneously
		desired = expected | observationToken;
	}

	registrationLock.Lock();	
	registeredObservers.emplace_back(const_cast<Observer*>(& observer));
	registrationLock.Unlock();

	return true;
}


///OBSERVER
Observer::Observer(const ObservationToken& observationToken, UniqueObserverRegister& uniqueObserverRegister)
	:observationToken{ observationToken }
{
	auto isUnique = uniqueObserverRegister.RegisterObserver(*this);
	assert(("Failed to assert Unique Observation Token", isUnique));
}

void Observer::ObserveEvent(Message&& message)
{
	messageLock.Lock();
	observedEventMessages.emplace_back(std::move(message));
	messageLock.Unlock();
}

ProcessMessage Observer::ProcessEvents(const MessageProcessingFunction& processFunction)
{
	messageLock.Lock();
	std::vector<Message> messages = std::move(observedEventMessages);
	messageLock.Unlock();

	return processFunction(messages);		
}//messages retaining ownership of messageBase release memory now