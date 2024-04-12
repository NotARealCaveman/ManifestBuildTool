#pragma once
#include <cassert>
#include <type_traits>

#include <ManifestUtility/Typenames.h>
#include <ManifestUtility/TypeAssist.h>//includes UnderlyingType

using namespace Manifest_Utility;

namespace Manifest_Communication
{
	//there is allowed up to 64 possible flags
	constexpr MFu64 pow2(MFu64 base)
	{
		return MFu64{ 1 } << base ;
	}
	constexpr MFu8 ipow2(MFu64 pow2Base)
	{
		MFu8 result{ 0 };
		for (; pow2Base > 1; ++result)
			pow2Base >>= 1;
		return result;
	}	
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