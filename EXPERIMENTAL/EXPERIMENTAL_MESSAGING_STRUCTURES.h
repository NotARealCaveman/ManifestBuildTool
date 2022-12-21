#pragma once
#include <vector>

#include <ManifestGLPersistence/DatabaseTypes.h>
#include <ManifestGLUtility/DebugLogger.h>

//will be implementing an subscription-subscriber pattern to handle cross framework messaging
namespace Manifest_Experimental
{
	constexpr MFu32 NEW_MESSAGE{ 1 };
	constexpr MFu32 ANOTHER_MESSAGE{ 2 };
	
	struct Message
	{
		MFu32 singal{ UINT32_MAX };
	};

	struct Event
	{
		Message message;
	};

	struct Subscriber
	{
		void ReceiveMessage(const Message& message);
		void ProcessMessage(const Message& message);
	};

	struct Distributer
	{
		void DistributeMessage();

		std::vector<Message> messages;		
		std::vector<Subscriber*> subscribers;
	};

	struct Subscription
	{
		void PublishMessage(const Event& event);

		Distributer* distributer;
	};	
}