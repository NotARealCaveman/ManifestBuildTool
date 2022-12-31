#include "EXPERIMENTAL_MESSAGING_STRUCTURES.h"

using namespace Manifest_Experimental;

void Publisher::PublishMessage(const Message& message)
{	
	DLOG(31, "Subscription pushing message");
	distributer->messages.emplace_back(message);
}

void Distributer::DistributeMessages()
{
	//distribute pending messages
	DLOG(32, "Distributing messages");
	for (const auto& subscriber : subscribers)
		for (const auto& message : messages)
			subscriber->ReceiveMessage(message);
	//remove pending messages
	messages.clear();
}

void Subscriber::ReceiveMessage(const Message& message)
{
	DLOG(33, "Message received");
	ProcessMessage(message);
}