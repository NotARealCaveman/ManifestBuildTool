#pragma once
#include <cassert>
#include <type_traits>

#include <ManifestUtility/Typenames.h>
#include <ManifestUtility/TypeAssist.h>//includes UnderlyingType

using namespace Manifest_Utility;

namespace Manifest_Communication
{	
	//generally - checks if an event is of interest to an Observer
	template<typename MessageTypes>
	inline constexpr MessageTypes operator&(const MessageTypes& observationToken, const MessageTypes& eventToken)
	{
		return static_cast<MessageTypes>(UnderlyingType(observationToken) & UnderlyingType(eventToken));
	}
	//generally - concatonates ObservationToken of an event 
	template<typename MessageTypes>
	inline constexpr MessageTypes operator|(const MessageTypes& eventToken, const MessageTypes& messageType)
	{
		return static_cast<MessageTypes>(UnderlyingType(eventToken) | UnderlyingType(messageType));
	}
}