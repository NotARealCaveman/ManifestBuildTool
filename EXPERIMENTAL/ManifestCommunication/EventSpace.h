#pragma once
#include <vector>
#include <algorithm>
#include <atomic>

#include "Observer.h"

namespace Manifest_Communication
{
	template<typename ObservableSystem>
	class EventSpace
	{			
		private:
			using EventObserver = Observer<ObservableSystem>;
			using Event = ObservableEvent<ObservableSystem>;
			using EventMessage = EventMessage<ObservableSystem>;
			
			mutable std::vector<Event> events;
		public:			
			void RecordEvent(Event&& event)
			{
				events.emplace_back(std::move(event));			
			}		
			void ObserveEvents(EventObserver& observer)
			{
				//for each event in the list of events
				for(Event& event : events)
				{//check if the event has any messages of interest
					if (!UnderlyingType(observer.observationToken & event.eventToken))
						return;					
					for (EventMessage& message : event.messages)
					{
						if (!UnderlyingType(observer.observationToken & message.first))
							continue;

					observer.observedEventMessages.emplace_back(std::move(message));
					}
				}
			}	
			std::atomic<MFu64> currentObservations;
			std::
	};
}