#pragma once
#include <functional>

namespace Manifest_Communication
{
	template<typename... Args>
	struct Trigger
	{
	private:
		Trigger() = delete;
		const std::function<void(Args...)> action;
	public:
		Trigger(const std::function<void(Args...)>& function)
			: action{ function } {};
		constexpr void operator()(Args&&... args) const
		{
			action(args...);
		};
	};

	typedef Trigger<const int&> LoadTrigger;

	template<typename ObservableGroup>
	struct ObservableGroupTriggers
	{
		static const LoadTrigger loadTrigger;
	};
	//hook up obersvable group actions to their triggers
	template<typename ObservableGroup>
	const LoadTrigger ObservableGroupTriggers< ObservableGroup>::loadTrigger{ ObservableGroup::OnLoad };
}