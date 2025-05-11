#include "Message.h"

using namespace Manifest_Communication;

Message::Message(Message&& other) noexcept
	: messageToken{ other.messageToken } , messageBase{ std::move(other.messageBase) }
{
	other.messageBase = nullptr;
}

Message::~Message()
{
	if (messageBase)		
		delete messageBase;
}