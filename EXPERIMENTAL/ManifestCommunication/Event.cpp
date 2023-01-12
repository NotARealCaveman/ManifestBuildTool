#include "Event.h"

using namespace Manifest_Communication;

Message::~Message()
{
	if (messageBase)
		delete messageBase;
}

Message::Message(Message&& other) noexcept
	:messageBase{ std::move(other.messageBase) }
{
	other.messageBase = nullptr;
}

void Message::CopyMessageContents(void* copyAddress)
{	
	messageBase->CopyContents(copyAddress);
}