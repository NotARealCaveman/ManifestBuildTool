#pragma once
#include <memory>

namespace Manifest_Experimental
{
	struct FunctionWrapper
	{
	private:
		struct FunctionBase
		{
			virtual void call() = 0;
			virtual ~FunctionBase() {};
		};
		template<typename Function>
		struct Wrapper : FunctionBase
		{			
			Wrapper(Function&& f) : wrapped{ std::move(f) } {};
			void call() { wrapped(); };
			~Wrapper() {};

			Function wrapped;
		};

		std::unique_ptr<FunctionBase> function;
	public:
		FunctionWrapper() = default;
		template<typename Function>
		FunctionWrapper(Function&& _function)
			:function{std::make_unique<Wrapper<Function>>(std::move(_function))}
		{};
		 FunctionWrapper(FunctionWrapper&& other) noexcept
			:function{ std::move(other.function) } {};
		FunctionWrapper& operator= (FunctionWrapper&& other)noexcept
		{
			function = std::move(other.function);
			return *this;
		}
		FunctionWrapper(const FunctionWrapper&) = delete;
		FunctionWrapper(FunctionWrapper&) = delete;
		FunctionWrapper operator =(const FunctionWrapper&) = delete;

		void operator()() const
		{
			function->call();
		}		
	};
}
