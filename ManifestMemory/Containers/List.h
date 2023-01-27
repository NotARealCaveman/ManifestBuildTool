#pragma once
#include <mutex>
#include <memory>
#include <algorithm>

#include "Iterator.h"

namespace Manifest_Memory
{	
	template<typename T>
	class ThreadedList
	{		
	private:
		ListNode<T> head;
	public:
		ThreadedList()
		{}
		~ThreadedList()
		{
			RemoveIf([](ListNode<T> const&) {return true; });
		}
		ThreadedList(ThreadedList const& other) = delete;
		ThreadedList& operator=(ThreadedList const& other) = delete;		
		void PushFront(T const& value)
		{
			std::unique_ptr<ListNode<T>> new_node(new ListNode<T>(value));
			std::lock_guard<std::mutex> lk(head.exclusiveNode);
			new_node->next = std::move(head.next);
			head.next = std::move(new_node);
		}
		template<typename Function>
		void ForEach(Function f)
		{
			ListNode<T>* current = &head;
			std::unique_lock<std::mutex> lk(head.exclusiveNode);
			while (ListNode<T>* const next = current->next.get())
			{
				std::unique_lock<std::mutex> next_lk(next->exclusiveNode);
				lk.unlock();
				f(*next->data);
				current = next;
				lk = std::move(next_lk);
			}
		}
		template<typename Predicate>
		std::shared_ptr<T> FindFirstIf(Predicate p)
		{
			ListNode<T>* current = &head;
			std::unique_lock<std::mutex> lk(head.exclusiveNode);
			while (ListNode<T>* const next = current->next.get())
			{
				std::unique_lock<std::mutex> next_lk(next->exclusiveNode);
				lk.unlock();
				if (p(*next->data))
				{
					return next->data;
				}
				current = next;
				lk = std::move(next_lk);
			}
			return std::shared_ptr<T>();
		}
		template<typename Predicate>
		void RemoveIf(Predicate p)
		{
			ListNode<T>* current = &head;
			std::unique_lock<std::mutex> lk(head.exclusiveNode);
			while (ListNode<T>* const next = current->next.get())
			{
				std::unique_lock<std::mutex> next_lk(next->exclusiveNode);
				if (p(*next->data))
				{
					std::unique_ptr<ListNode<T>> old_next = std::move(current->next);
					current->next = std::move(next->next);
					next_lk.unlock();
				}
				else
				{
					lk.unlock();
					current = next;
					lk = std::move(next_lk);
				}
			}
		}			
		ListItr<T> begin()
		{
			std::unique_lock<std::mutex> lk(head.exclusiveNode);
			return head.next.get();		
		}		
		ListItr<T> end()
		{
			std::unique_lock<std::mutex> lk(head.exclusiveNode);
			ListNode<T>* itr = head.next.get();
			while (itr)
				itr = itr->next.get();
			return ListItr<T>{itr};
		}
		ListItr<T> lastElem()
		{
			std::unique_lock<std::mutex> lk(head.exclusiveNode);
			ListNode<T>* itr = head.next.get();
			while (itr->next)
				itr = itr->next.get();
			return ListItr<T>{itr};
		}
	};
	
}