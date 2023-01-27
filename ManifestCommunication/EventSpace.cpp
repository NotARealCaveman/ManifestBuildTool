#include "EventSpace.h"

using namespace Manifest_Communication;

//requires at least the observers which make up the event token to be registered before recving the event
void EventSpace::NotifyRegisteredObservers(Event&& event)
{	
	//this is a potential race condition if an observer is regsitered at during the call - however will wait to see if an actual issue. further development required to make decision if observers will all be registered at initialization stages
	for (auto& message : event.messages)
	{
		auto& observers = observerRegister.registeredObservers;
		auto observer = std::find_if(observers.begin(), observers.end(), [&](Observer* observer)
			{return (observer->observationToken & message.messageToken); });
		assert(("no observer", observer != observers.end()));
		(*observer)->ObserveEvent(std::move(message));
	}
}