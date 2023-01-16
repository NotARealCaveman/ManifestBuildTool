#include "Event.h"

using namespace Manifest_Communication;

ObservableEvent::ObservableEvent(ObservableEvent&& other) noexcept
	: eventToken{ other.eventToken.load(std::memory_order_relaxed)}, messages{std::move(other.messages)} {}

void Manifest_Communication::ClearObservedMessages(const ObservationToken& observationToken, std::atomic<ObservationToken>& eventToken)
{	
	auto expected = eventToken.load(std::memory_order_acquire);
	auto desired = expected & ~observationToken;
	while (!eventToken.compare_exchange_strong(expected, desired, std::memory_order_release, std::memory_order_relaxed))
		desired = expected & ~observationToken;
}