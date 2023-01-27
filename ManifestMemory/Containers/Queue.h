#pragma once
#include <mutex>
#include <condition_variable>

namespace Manifest_Memory
{
	template<typename T>
	class ThreadedQueue
	{
	private:
		using shared_ptr = std::shared_ptr<T>;
		struct QueueNode;
		using unique_ptr = std::unique_ptr<QueueNode>;
		struct QueueNode
		{
			shared_ptr data;
			unique_ptr next;
		};

		QueueNode* GetTail() const
		{
			std::lock_guard<std::mutex> tailLock(exclusiveTail);
			return tail;
		}
		unique_ptr PopHead()
		{
			unique_ptr oldHead = std::move(head);
			head = std::move(oldHead.get()->next);
			return oldHead;
		}
		unique_ptr TryPopHead(T& value)
		{
			std::lock_guard<std::mutex > headLock(exclusiveHead);
			if (head.get() == GetTail())
				return unique_ptr();
			value = std::move(*head->data);
			return PopHead();
		}
		unique_ptr TryPopHead()
		{
			std::lock_guard<std::mutex> headLock(exclusiveHead);
			if (head.get() == GetTail())
				return unique_ptr();
			return PopHead();
		}
		std::unique_lock<std::mutex> WaitForData()
		{
			std::unique_lock<std::mutex> headLock(exclusiveHead);
			queueCondition.wait(headLock, [&] {return head.get() != GetTail(); });
			return std::move(headLock);
		}
		unique_ptr WaitPopHead()
		{
			std::unique_lock<std::mutex> headLock(WaitForData());
			return PopHead();
		}
		unique_ptr WaitPopHead(T& value)
		{
			std::unique_lock<std::mutex> headLock(WaitForData());
			value = std::move(*head->data);
			return PopHead();
		}		

		mutable std::mutex exclusiveHead;
		mutable	std::mutex exclusiveTail;
		std::condition_variable queueCondition;
		unique_ptr head;
		QueueNode* tail;
	public:
		ThreadedQueue()
			:head{new QueueNode }
		{
			tail = head.get();
		}
		ThreadedQueue(const ThreadedQueue& other) = delete;
		ThreadedQueue& operator=(const ThreadedQueue&) = delete;

		//pushes a copy of the value onto the stack as a unique_ptr to a node with a shared_pt to some T
		void Push(T value)
		{		
			shared_ptr data(std::make_shared<T>(std::move(value)));
			unique_ptr node(new QueueNode);
			QueueNode* const newTail = node.get();
		//scope lock lifetime
			{	std::lock_guard<std::mutex> tailLock(exclusiveTail);
			tail->data = data;
			tail->next = std::move(node);
			tail = newTail;
			}
		//notify if any, no need for lock
			queueCondition.notify_one();
		}
		//attempts to pop the top value from the queue. if one exists its stored in value. uses presence of pointer as boolean
		bool TryPop(T& value)
		{
			const unique_ptr oldHead = TryPopHead(value);
			return oldHead.get();
		}
		//attempts to pop the top value from the queue. if one exists its stored on the stack in a unique_ptr and returns the shared_ptr to some T data
		shared_ptr TryPop()
		{
			unique_ptr oldHead = TryPopHead();
			return *oldHead ? oldHead.get()->data : shared_ptr();
		}	
		
		void WaitPop(T& value)
		{
			const unique_ptr oldHead = WaitPopHead(value);			
		}
		shared_ptr WaitPop()
		{
			const unique_ptr oldHead = WaitPopHead();
			return oldHead.get()->data;
		}
		
		bool Empty() const
		{
			std::lock_guard<std::mutex> headLock(exclusiveHead);
			return(head.get() == GetTail());
		}		
	};
}