#pragma once
#include <iostream>
#include <memory>
#include <atomic>

#include "MessageOperators.h"

namespace Manifest_Communication
{
	//Messages will be paired with EventSpace<ObserveableSystem>::MessageTypes
	//Messages are type erased containers - this allows multiple messages with varying types of contents to be placed into the same Event snapshot
	//Messages are copy only - EventSpace<ObserveableSystem>::MessageTypes may only have one observer per type of message, though there may up to be N observers of an event space;N = number of Message Types
	struct Message
	{
		struct MessageBase
		{			
			virtual void GetContents(void* copyAddress) const = 0;
		};
		template<typename Content>
		struct ContentWrapper : MessageBase
		{				
			ContentWrapper(const Content& _content)
				: content{ _content } {};
			ContentWrapper(ContentWrapper&& other)
				: content{ std::move(other.content) } {};
			ContentWrapper(const ContentWrapper& other) = delete;
			//moves msg contents to object supplied by observer
			void GetContents(void* copyAddress) const { memcpy(copyAddress, &content, sizeof(Content)); };
			const Content content;
		};		
		const MessageBase* messageBase;
		template<typename Content>
		Message(const Content& content)
			: messageBase{ new ContentWrapper{content} } {};
		Message(Message&& other) noexcept;
		Message(const Message& other) = delete;		
		~Message();		
		//invokes move semantics from message to observer object
		void GetMessageContents(void* copyAddress);		
	};
};