#include "EXPERIMENTAL_MESSAGING_STRUCTURES.H"

using namespace Manifest_Experimental;

void Subscription::PublishMessage(const Event& event)
{	
	DLOG(31, "Subscription pushing message");
	distributer->messages.emplace_back(event.message);
}

void Distributer::DistributeMessage()
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

void Subscriber::ProcessMessage(const Message& message)
{
	DLOG(34, "Singal: " << message.singal);
}