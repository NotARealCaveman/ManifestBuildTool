#include "Observer.h"

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
			return false;
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

void Observer::ObserveEvent(EventMessage&& eventMessage)
{
	messageLock.Lock();
	observedEventMessages.emplace_back(std::move(eventMessage));
	messageLock.Unlock();
}

void Observer::ProcessEvents(const MessageProcessingFunction& processFunction)
{
	messageLock.Lock();
	std::vector<EventMessage> messages = std::move(observedEventMessages);
	messageLock.Unlock();

	processFunction(messages,this);
}