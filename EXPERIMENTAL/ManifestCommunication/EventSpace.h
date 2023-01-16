#pragma once
#include <vector>
#include <algorithm>

#include "Event.h"

namespace Manifest_Communication
{
	class EventSpace
	{			
		private:				
			mutable std::vector<Event> events;
		public:			
			void RecordEvent(Event&& event);
			//processes events based on observation token and removes bits from observed event's event token
			void ObserveEvents(const ObservationToken& observationToken, std::vector<EventMessage>& eventMessages);
	};

}