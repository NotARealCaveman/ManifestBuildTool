#pragma once
#include <vector>
#include <thread>

#include <EXPERIMENTAL/Typenames.h>
#include <ManifestGLUtility/DebugLogger.h>

using namespace Manifest_Experimental;

namespace Manifest_Communication
{	
	//allows a compile time token to be created that allow access to specific event space messages;
	template<typename ObservableSystem>
	using ObserverationToken = enum ObservableSystem::MessageTypes;
	

	template<typename ObservableSystem>
	struct Event
	{			
		std::vector<enum ObservableSystem::MessageTypes> messages;
	};

	template<typename ObservableSystem>
	struct EventSpace
	{		
		//write sync stuff
		static std::vector<Event<ObservableSystem>> events;
		//RCU stuff		
		/*...*/		
		static std::vector<enum ObservableSystem::MessageTypes> ObserveEventMessages(const ObserverationToken<ObservableSystem>& observerationToken)
		{
			std::vector<enum ObservableSystem::MessageTypes> result;
			for (const auto& event : events)
				for (const auto& message : event.messages)
					if (UnderlyingType(observerationToken & message))
						result.emplace_back(message);
			return result;
		}
	};
	template<typename ObservableSystem>
	std::vector<Event<ObservableSystem>> EventSpace<ObservableSystem>::events;
}