#pragma once
#include <iostream>
#include <memory>
#include <atomic>

#include "MessageOperators.h"

namespace Manifest_Communication
{
	//token that allow access to specific event space messages
	using ObservationToken = MFu64;

	//Messages are type erased containers - this allows multiple messages with varying types of contents to be placed into the same Event snapshot	
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
			ContentWrapper(Content&& _content)
				: content{ std::move(_content) } {};
			ContentWrapper(ContentWrapper&& other)
				: content{ std::move(other.content) } {};
			ContentWrapper(const ContentWrapper& other) = delete;
			//moves msg contents to object supplied by observer
			void GetContents(void* copyAddress) const { memcpy(copyAddress, &content, sizeof(Content)); };
			const Content content;
		};				
		template<typename Content>
		Message(const ObservationToken& _messageToken, const Content& content)
			: messageToken{ _messageToken }, messageBase{ new ContentWrapper{content} } {};
		template<typename Content>
		Message(const ObservationToken& _messageToken, Content&& content)
			: messageToken{ _messageToken }, messageBase{ new ContentWrapper{std::move(content)} } {};
		Message(Message&& other) noexcept;
		Message(const Message& other) = delete;		
		~Message();		
		//invokes move semantics from message to observer object
		void GetMessageContents(void* copyAddress);		

		const MessageBase* messageBase; 
		const ObservationToken messageToken;				
	};
};