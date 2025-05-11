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
		struct MessageBase{};
		template<typename Content>
		struct ContentWrapper : MessageBase
		{	
			ContentWrapper(const ContentWrapper& other) = delete;
			ContentWrapper(const Content& _content)
				: content{ _content } {};
			ContentWrapper(Content&& _content)
				: content{ std::move(_content) } {};
			ContentWrapper(ContentWrapper&& other)
				: content{ std::move(other.content) }
			{
				other.content = nullptr;
			};

			Content content;
		};				
		template<typename Content>
		Message(const ObservationToken& _messageToken, const Content& content)
			: messageToken{ _messageToken }, messageBase{ new ContentWrapper{content} }{};
		template<typename Content>
		Message(const ObservationToken& _messageToken, Content&& content)
			: messageToken{ _messageToken }, messageBase{ new ContentWrapper{std::move(content)} }{};
		Message(Message&& other) noexcept;
		Message(const Message& other) = delete;		
		~Message();				
		//dynamic allocations in content are not cleaned up by ~Message(), only messageBase itself
		template<typename Content>		
		Content& GetMessageContent() const
		{
			ContentWrapper<Content>* contentWrapper = static_cast<ContentWrapper<Content>*>(messageBase);	

			Content& content{ contentWrapper->content };

			return content;
		}
			
		const ObservationToken messageToken;						
		MessageBase* messageBase;		
	};	
};