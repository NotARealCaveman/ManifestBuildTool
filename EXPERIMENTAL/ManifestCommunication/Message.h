#pragma once
#include <iostream>
#include <memory>

#include "MessageOperators.h"

namespace Manifest_Communication
{
	struct Message
	{
		struct MessageBase
		{
			virtual void CopyContents(void* copyAddress) const = 0;
		};
		template<typename Payload>
		struct PayloadWrapper : MessageBase
		{				
			PayloadWrapper(const Payload& contents)
				: payload{ contents } {};
			PayloadWrapper(PayloadWrapper&& other)
				: payload{ std::move(other.payload) } {};
			PayloadWrapper(const PayloadWrapper& other) = delete;
			void CopyContents(void* copyAddress) const { memcpy(copyAddress, &payload, sizeof(Payload)); };
			const Payload payload;
		};		
		const MessageBase* messageBase;
		template<typename Payload>
		Message(const Payload& payload)
			: messageBase{ new PayloadWrapper{payload} } {};	
		Message(Message&& other) noexcept;
		Message(const Message& other) = delete;		
		~Message();
		void CopyMessageContents(void* copyAddress);		
	};
};