#pragma once 
#include <iterator>

#include <ManifestGLUtility/DebugLogger.h>

namespace Manifest_Memory
{
	template<typename T>
	struct ListNode
	{
		mutable std::mutex exclusiveNode;
		std::shared_ptr<T> data;
		std::unique_ptr<ListNode> next;
		ListNode() :
			next()
		{}
		ListNode(T const& value) :
			data(std::make_shared<T>(value))
		{}
	};

	template<typename T>
	class ListItr
	{
		private:
			ListNode<T>* itr;			
		public:
			ListItr(ListNode<T>* listItr=nullptr) :itr{ listItr } {};
			ListItr& operator++()
			{
				this->itr = itr->next.get();
				return *this;
			}
			ListItr operator++(int)
			{
				ListItr next = *this;
				*this = ++next;
				return next;
			}
			T& operator*() const { return *itr->data.get(); };
			T& operator->() const { return itr->data.get(); };
			bool operator==(const ListItr& other) const { return itr == other.itr; };
			bool operator!=(const ListItr& other) const { return itr != other.itr; };
	};
	template<typename Itr>
	const uint32_t distance(Itr first, Itr last)
	{
		uint32_t result = 0;
		for (; first != last; ++result,++first);
		return result;
	}
	template<typename Itr>
	void advance(Itr& begin, uint32_t length)
	{
		for (; length-- > 0; ++begin);
	}
	template<typename Itr, typename T>
	Itr find(Itr first, Itr last, const T& search)
	{
		for (; first != last; ++first) 
			if (*first == search)
				return first;		
		return last;
	}
	template<typename Itr, typename Predicate>
	Itr find_if(Itr first, Itr last, Predicate p)
	{
		for (; first != last; ++first)
			if (p(*first))
				return first;
		return last;
	}
}