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

	template<typename E>
	inline constexpr typename std::underlying_type<E>::type UnderlyingType(const E& e)
	{
		return static_cast<typename std::underlying_type<E>::type>(e);
	};

	template<typename MessageTypes>
	inline constexpr MessageTypes operator&(const MessageTypes& typeA, const MessageTypes& typeB)
	{
		return static_cast<MessageTypes>(UnderlyingType(typeA) & UnderlyingType(typeB));
	}
	template<typename MessageTypes>
	inline constexpr MessageTypes operator|(const MessageTypes& typeA, const MessageTypes& typeB)
	{
		return static_cast<MessageTypes>(UnderlyingType(typeA)
			| UnderlyingType(typeB));
	}
}