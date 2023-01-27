#pragma once
#include <memory> 
#include <mutex>
#include <stack>

namespace Manifest_Memory
{
	template<typename T>
	class ThreadedStack
	{//some comment
	private:
		std::stack<T> stack;
		mutable std::mutex exclusiveStack;
	public:
		ThreadedStack() = default;
		ThreadedStack(const ThreadedStack& other)
		{
			std::lock_guard<std::mutex> stackLock(other.exclusiveStack);
			stack = other.stack;
		}
		ThreadedStack& operator=(const ThreadedStack&) = delete;
		void Push(T new_value)
		{
			std::lock_guard<std::mutex> stackLock(exclusiveStack);
			stack.push(new_value);
		}
		void Pop(std::shared_ptr<T>& ptr)
		{
			std::lock_guard<std::mutex> stackLock(exclusiveStack);
			if (stack.empty())
				return;
			ptr = std::make_shared<T>(stack.top());
			stack.pop();
		}
		void Pop(T& value)
		{
			std::lock_guard<std::mutex> stackLock(exclusiveStack);
			if (stack.empty())
				return;
			value = stack.top();
			stack.pop();
		}
		bool Empty() const
		{
			std::lock_guard<std::mutex> stackLock(exclusiveStack);
			return stack.empty();
		}
	};
}