#include "Message.h"

using namespace Manifest_Communication;

Message::~Message()
{
	if (messageBase)
		delete messageBase;
}

Message::Message(Message&& other) noexcept
	: messageBase{ std::move(other.messageBase) }
{
	other.messageBase = nullptr;
}

void Message::GetMessageContents(void* copyAddress)
{
	messageBase->GetContents(copyAddress);
}