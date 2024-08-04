#pragma once
#include <functional>	
#include <memory>

#include <ManifestUtility/DebugLogger.h>
#include <ManifestUtility/TypeAssist.h>

using namespace Manifest_Utility;

namespace Manifest_Memory
{
	struct FunctionBinder
	{
	public:
		FunctionBinder() = default;
		template<typename Function, typename... Params>
		FunctionBinder(Function&& function, Params&&... params)
			:callable{ std::make_unique<Callable<Function,Params...>>(std::forward<Function>(function),std::forward<Params>(params)...) } 
		{
			//DLOG({ CONSOLE_CYAN }, "Creating function binder:", this);
		};
		FunctionBinder(FunctionBinder&& other) : callable{ std::move(other.callable) }
		{ 
			//DLOG({ CONSOLE_CYAN }, "move ctor function binder:", this, "other:", &other); 		
		};
		FunctionBinder& operator=(FunctionBinder&& other)
		{
			//DLOG({ CONSOLE_CYAN }, "move operator= function binder:", this, "other:",&other);
			callable = std::move(other.callable);
			return *this; 
		};
		~FunctionBinder()
		{
			//DLOG({ CONSOLE_CYAN }, "Destroying function binder:", this);
		}
		void operator()() const
		{
			ForwardFunction(&FunctionBase::operator(), callable);	
		}
	private:
		FunctionBinder(const FunctionBinder&) = delete;
		FunctionBinder operator=(const FunctionBinder&) = delete;

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
			Callable(Callable&& other) : boundFunction{ std::move(other.boundFunction) } 
			{};
			Callable operator=(Callable&& other) {};
			Callable(const Callable&) = delete;
			Callable& operator=(const Callable&) = delete;
			~Callable() {};
			void operator()() 
			{
				ForwardFunction(boundFunction);				
			};			

			std::function<void()> boundFunction;
		};

		std::unique_ptr<FunctionBase> callable;
	};
}
