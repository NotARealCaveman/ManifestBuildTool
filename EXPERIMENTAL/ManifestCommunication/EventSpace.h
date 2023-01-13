#pragma once
#include <vector>
#include <algorithm>

#include "Observer.h"

namespace Manifest_Communication
{
	template<typename ObservableSystem>
	class EventSpace
	{			
		private:
			using EventObserver = Observer<ObservableSystem>;
			using Event = ObservableEvent<ObservableSystem>;

			mutable std::vector<Event> events;
		public:			
			void RecordEvent(Event&& event)
			{
				events.emplace_back(std::move(event));			
			}		
			void ObserveEvents(Observer<ObservableSystem>& observer)
			{
				//for each event in the list of events
				for (ObservableEvent<ObservableSystem>& event : events)
				{//check if the event has any messages of interest
					if (!UnderlyingType(observer.observationToken & event.eventToken))
						return;
					const EventInformation<ObservableSystem>& eventInformation = event.eventInformation;		
					for (auto messageIndex{ 0 }; messageIndex < eventInformation.messages.size(); ++messageIndex)
					{	//check if current message is of interest
						if (!UnderlyingType(observer.observationToken & eventInformation.messageTypes[messageIndex]))
							continue;
						//move message of interest
						//need finer granulairty in movement - currently moving entire event info. need to move only messages of ineterst
					}
				}
			}
	};
}