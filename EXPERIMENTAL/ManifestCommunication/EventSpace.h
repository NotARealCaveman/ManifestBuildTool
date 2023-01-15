#pragma once
#include <vector>
#include <algorithm>
#include <atomic>

#include "Event.h"

namespace Manifest_Communication
{
	//template<typename ObservableSystem>
	class EventSpace
	{			
		private:			
			using Event = ObservableEvent;// <ObservableSystem>;
			//using EventMessage = EventMessage<ObservableSystem>;
			//using ObserverationToken = ObserverationToken<ObservableSystem>;
			
			mutable std::vector<Event> events;
		public:			
			void RecordEvent(Event&& event)
			{
				events.emplace_back(std::move(event));			
			}		
			void ObserveEvents(const ObservationToken& observationToken, std::vector<EventMessage>& eventMessages)
			{
				//for each event in the list of events
				for(Event& event : events)
				{//check if the event has any messages of interest
					if (!(observationToken & event.eventToken))
						return;					
					for (EventMessage& message : event.messages)
					{
						if (!(observationToken & message.first))
							continue;
						ClearObservedMessages(event.eventToken, message.first);
						eventMessages.emplace_back(std::move(message));
					}
				}
			}	
			std::atomic<MFu64> currentObservations;
	};

}