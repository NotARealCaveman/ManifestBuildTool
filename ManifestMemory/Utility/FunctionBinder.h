#pragma once
#include <functional>	
#include <memory>


namespace Manifest_Memory
{
	struct FunctionBinder
	{
	public:
		FunctionBinder() = default;
		template<typename Function, typename... Params>
		FunctionBinder(Function&& function, Params&&... params)
			:callable{ std::make_unique<Callable<Function,Params...>>(std::forward<Function>(function),std::forward<Params>(params)...) } {};
		void operator()()
		{
			callable->operator()();
		}
	private:
		struct FunctionBase
		{
			virtual void operator()() = 0;
			virtual ~FunctionBase() {};
		};
		template<typename Function, typename... Params>
		struct Callable : public FunctionBase
		{
			Callable(Function&& function, Params&&... params)
				: boundFunction{ std::bind(function,params...) }
			{};
			void operator()()
			{
				std::invoke(boundFunction);
			};

			std::function<void()> boundFunction;
		};
		std::unique_ptr<FunctionBase> callable;
	};
}
