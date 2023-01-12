#pragma once
#include <vector>
#include <algorithm>

#include "Observer.h"

namespace Manifest_Communication
{
	template<typename ObservableSystem>
	struct EventSpace
	{
		using EventObserver = Observer<ObservableSystem>;

		std::vector<EventObserver*> observers;
		void BrokerEvent(const ObservableEvent<ObservableSystem>& event)
		{
			for (auto messageIndex{ 0 }; messageIndex < event.eventInformation.messages.size(); ++messageIndex)
			{				
				const auto& messageType = event.eventInformation.messageTypes[messageIndex];
				const auto& message = event.eventInformation.messages[messageIndex];
				for (auto& observer : observers)
					if ((bool)(observer->observationToken & messageType))
					{
						observer->observedEvents.messageTypes.emplace_back(messageType);
						//need to look into this further - lack of copy constructor is causing a problem
						observer->observedEvents.messages.emplace_back(std::move(message);
					}
			}				
		};
	};
}