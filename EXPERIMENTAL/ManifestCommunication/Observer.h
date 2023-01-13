#pragma once
#include <vector>
#include <thread>

#include "Event.h"

#include <EXPERIMENTAL/Typenames.h>
#include <ManifestGLUtility/DebugLogger.h>

using namespace Manifest_Experimental;

namespace Manifest_Communication
{
	//allows a compile time token to be created that allow access to specific event space messages;
	template<typename ObservableSystem>
	struct Observer
	{
		using ObservationToken = ObserverationToken<ObservableSystem>;
		using EventInformation = EventInformation<ObservableSystem>;

		Observer(const ObservationToken& token)
			:observationToken{ token } {}

		const ObservationToken observationToken;
		std::vector<EventInformation> observedEvents;
	};
}