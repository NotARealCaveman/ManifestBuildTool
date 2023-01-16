#include "EventSpace.h"

using namespace Manifest_Communication;

void EventSpace::RecordEvent(Event&& event)
{
	events.emplace_back(std::move(event));
}
//for each event - check if the event has messages we're observing
//for each message in event move message if observed
//if event was observed remove the observed bits from event token
void EventSpace::ObserveEvents(const ObservationToken& observationToken, std::vector<EventMessage>& eventMessages)
{
	bool observedEvents{ false };
	//for each event in the list of events
	for (Event& event : events)
	{//check if the event has any messages of interest
		if (!(observationToken & event.eventToken))
			continue;
		for (EventMessage& message : event.messages)
		{//check if specific message is of interest
			if (!(observationToken & message.first))
				continue;			
			eventMessages.emplace_back(std::move(message));
			observedEvents = true;
		}
		//clear observed messages from event 
		if(observedEvents)
			ClearObservedMessages(observationToken, event.eventToken);
	}
}