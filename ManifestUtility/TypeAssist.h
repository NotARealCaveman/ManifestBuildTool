#pragma once

#include <utility>
#include <type_traits>

namespace Manifest_Utility
{
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