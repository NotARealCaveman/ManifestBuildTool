#pragma once
#include <vector>
#include <utility>
#include <functional>


#include <ManifestGLPersistence/DatabaseTypes.h>
#include <ManifestGLUtility/DebugLogger.h>

//will be implementing an Publisher-Subscriber pattern to handle cross framework messaging
namespace Manifest_Experimental
{

	template<typename... Args>
	struct Trigger
	{
	private:
		Trigger() = delete;
		const std::function<void(Args...)> action;
	public:
		Trigger(const std::function<void(Args...)>& function)
			: action{ function } {};
		void operator()(Args&&... args) const
		{
			action(args...);
		};
	};
	
	struct Message
	{
		//message types will map to 
		enum class MessageType
		{
			LOAD
		}messageType;

	};

	struct Event
	{
		//event types should match each of the triggers available

	};
	
	//forward declared for ease of readability - allows event spaces to be defined with their observation groups		
	template<typename ObservableGroup>
	struct ObservableGroupTriggers;

	struct ObersvableFileSystem 
	{
		static void OnLoad(const int& loadEvent);		
	};	
	typedef
		ObservableGroupTriggers<ObersvableFileSystem> FileSystemTriggers;
	
	//predefined trigger types that may be implemented by observable groups
	typedef Trigger<const int&> LoadTrigger;	

	template<typename ObservableGroup>
	struct ObservableGroupTriggers
	{
		static const LoadTrigger loadTrigger;		
	};	
	//hook up obersvable group actions to their triggers
	template<typename ObservableGroup>
	const LoadTrigger ObservableGroupTriggers< ObservableGroup>::loadTrigger{ ObservableGroup::OnLoad };	
}