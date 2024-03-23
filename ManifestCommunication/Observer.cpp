#include "Observer.h"

using namespace Manifest_Communication;
///UNIQUE OBSERVER REGISTER
bool UniqueObserverRegister::RegisterObserver(const Observer& observer)
{
	if (registeredObservationTokens & observer.observationToken)
		return false;
	registeredObservationTokens |= observer.observationToken;
	registeredObservers.emplace_back(const_cast<Observer*>(&observer));

	return true;
}


///OBSERVER
Observer::Observer(const ObservationToken& observationToken)
	:observationToken{ observationToken } {};

void Observer::ObserveEvent(Message&& message)
{
	messageLock.Lock();
	observedEventMessages.emplace_back(std::move(message));	
	messageLock.Unlock();
}
