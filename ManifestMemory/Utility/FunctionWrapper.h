#pragma once
#include <functional>	
#include <memory>

#include <ManifestUtility/DebugLogger.h>
#include <ManifestUtility/TypeAssist.h>

using namespace Manifest_Utility;


namespace Manifest_Memory
{ 
	struct FunctionWrapper_OLD
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
		FunctionWrapper_OLD() = default;
		template<typename Function>
		FunctionWrapper_OLD(Function&& _function)
			:function{std::make_unique<Wrapper<Function>>(std::move(_function))}
		{};
		FunctionWrapper_OLD(FunctionWrapper_OLD&& other) noexcept
			:function{ std::move(other.function) } {};
		FunctionWrapper_OLD& operator= (FunctionWrapper_OLD&& other)noexcept
		{
			function = std::move(other.function);
			return *this;
		}
		FunctionWrapper_OLD(const FunctionWrapper_OLD&) = delete;
		FunctionWrapper_OLD(FunctionWrapper_OLD&) = delete;
		FunctionWrapper_OLD operator =(const FunctionWrapper_OLD&) = delete;

		void operator()() const
		{
			function->call();
		}		
	};
}
