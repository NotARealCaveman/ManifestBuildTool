#include "Observer.h"

using namespace Manifest_Communication;

bool UniqueObserverRegister::RegisterObserver(const ObservationToken& observationToken)
{
	auto expected = registeredObservationTokens.load(std::memory_order_acquire);
	if (expected & observationToken)
		return false;
	auto desired = expected | observationToken;
	while (!registeredObservationTokens.compare_exchange_strong(expected, desired, std::memory_order_release, std::memory_order_relaxed))
		desired = expected | observationToken;	

	return true;
}

Observer::Observer(const ObservationToken& observationToken, UniqueObserverRegister& uniqueObserverRegister)
	:observationToken{ observationToken }
{
	auto isUnique = uniqueObserverRegister.RegisterObserver(observationToken);
	assert(("Failed to assert Unique Observation Token", isUnique));
}