#include "Event.h"

using namespace Manifest_Communication;

ObservableEvent::ObservableEvent(ObservableEvent&& other) noexcept
	: eventToken{ other.eventToken }, messages{ std::move(other.messages) } {}

void Manifest_Communication::ClearObservedMessages(const MFu64& messageType, ObservationToken& eventToken)
{	
	eventToken &= ~(MFu64{ 1 } << DecodeMessageBase(messageType));
}