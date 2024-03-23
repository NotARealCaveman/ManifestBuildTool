#include "Event.h"

using namespace Manifest_Communication;

ObservableEvent::ObservableEvent(ObservableEvent&& other) noexcept 
	: eventToken{ other.eventToken}, messages{std::move(other.messages)}
{}