#include "Message.h"

using namespace Manifest_Communication;

Message::Message(Message&& other) noexcept
	: messageBase{ std::move(other.messageBase) }, messageToken{ other.messageToken }
{
	other.messageBase = nullptr;
}

Message::~Message()
{
	if (messageBase)
		delete messageBase;
}


void Message::GetMessageContents(void* copyAddress)
{
	messageBase->GetContents(copyAddress);
}