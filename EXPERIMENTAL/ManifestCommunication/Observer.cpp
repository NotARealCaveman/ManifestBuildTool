#include "Observer.h"

using namespace Manifest_Communication;

bool UniqueObserverRegister::RegisterObserver(const ObservationToken& observationToken)
{
	if (registeredObservationTokens & observationToken)
		return false;
	registeredObservationTokens |= observationToken;

	return true;
}

Observer::Observer(const ObservationToken& observationToken, UniqueObserverRegister& uniqueObserverRegister)
	:observationToken{ observationToken }
{
	auto isUnique = uniqueObserverRegister.RegisterObserver(observationToken);
	assert(("Failed to assert Unique Observation Token", isUnique));
}