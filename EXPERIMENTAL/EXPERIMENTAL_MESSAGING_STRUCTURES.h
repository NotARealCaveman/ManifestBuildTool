#pragma once
#include <vector>

#include <ManifestGLPersistence/DatabaseTypes.h>
#include <ManifestGLUtility/DebugLogger.h>

//will be implementing an Publisher-Subscriber pattern to handle cross framework messaging
namespace Manifest_Experimental
{

	typedef MFu32 Message;	
	constexpr Message NEW_MESSAGE{ 1 };
		

	struct Subscriber
	{
		void ReceiveMessage(const Message& message);
		virtual void ProcessMessage(const Message& message)=0;
	};
	struct ISub :public Subscriber
	{
		void ProcessMessage(const Message& message) { };
	};

	struct Distributer
	{
		void DistributeMessages();

		std::vector<Message> messages;
		std::vector<Subscriber*> subscribers;
	};

	struct Publisher
	{
		void PublishMessage(const Message& message);

		Distributer* distributer;
	};	

	struct PhysicsSubscriber : public Subscriber
	{
		void ProcessMessage(const Message& message) { };
	};
}