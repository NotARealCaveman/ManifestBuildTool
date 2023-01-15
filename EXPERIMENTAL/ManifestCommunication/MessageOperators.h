#pragma once
#include <type_traits>
#include <EXPERIMENTAL/Typenames.h>

using namespace Manifest_Experimental;

namespace Manifest_Communication
{
	//there is allowed up to 64 possible flags
	constexpr MFu64 pow2(MFu64 base, MFu64 result = 1)
	{
		for (; base; base--, result <<= 1); return result;
	}
	//generally - will return the MFu64 value of the MessageType
	template<typename E>
	inline constexpr typename std::underlying_type<E>::type UnderlyingType(const E& e)
	{
		return static_cast<typename std::underlying_type<E>::type>(e);
	};
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