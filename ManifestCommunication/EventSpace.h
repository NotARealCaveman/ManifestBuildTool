#pragma once
#include <vector>
#include <algorithm>

#include "Observer.h"

namespace Manifest_Communication
{	
	//Eventspaces are simply event brokers
	//they have the cavet that they must be fully subscribed to the degree of the event token they're recving before they may notify of an event
	class EventSpace
	{			
		public:		
			//Doles out event messages based on observer tokens
			void NotifyRegisteredObservers(Event&& event) const;

			UniqueObserverRegister observerRegister;
	};
	MFbool RegisterObserverToEventSpace(const Observer& observer, EventSpace& eventSpace);	
}