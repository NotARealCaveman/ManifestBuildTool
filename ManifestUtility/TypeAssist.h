#pragma once

#include <cstdint>
#include <utility>
#include <type_traits>

namespace Manifest_Utility
{
	//there is allowed up to 64 possible flags
	constexpr uint64_t pow2(const uint64_t base)
	{
		return uint64_t{ 1 } << base;
	}
	constexpr uint16_t ipow2(const uint64_t pow2Base)
	{
		return pow2Base>>1;
	}
	constexpr uint8_t root2(uint64_t pow2Base)
	{
		uint8_t result{ 0 };
		while (pow2Base = pow2Base >> 1)
			++result;
		return result;
	}	

	template<typename E>
	inline constexpr typename std::underlying_type<E>::type UnderlyingType(const E& e)
	{
		return static_cast<typename std::underlying_type<E>::type>(e);
	};

	template<typename Function, typename... Params>
	using ReturnType = decltype(std::declval<Function>()(std::declval<Params>()...));

	template<typename Function, typename... Params>
	ReturnType<Function, Params...> ForwardFunction(Function&& function, Params&&... params)
	{
		return std::invoke(std::forward<Function>(function), std::forward<Params>(params)...);
	};
}