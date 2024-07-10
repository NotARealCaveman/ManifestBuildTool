#pragma once
#include <functional>	
#include <memory>

#include <ManifestUtility/DebugLogger.h>

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
		void operator()()
		{
			callable->operator()();
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
			{
				//DLOG({ CONSOLE_CYAN }, "Creating Callable:", this);
			};
			Callable(Callable&& other) : boundFunction{ std::move(other.boundFunction) } 
			{ //DLOG({ CONSOLE_CYAN }, "move ctor callable:", this, "other:", &other); 
			};
			Callable operator=(Callable&& other)
			{ //DLOG({ CONSOLE_CYAN }, "operator= callable:", this, "other:", &other); return std::move(other); 
			};
			Callable(const Callable&) = delete;
			Callable& operator=(const Callable&) = delete;

			~Callable()			
			{ //DLOG({ CONSOLE_CYAN }, "Destroying callable:", this);
			};
			void operator()()
			{
				std::invoke(boundFunction);
			};

			std::function<void()> boundFunction;
		};
		std::unique_ptr<FunctionBase> callable;
	};
}
