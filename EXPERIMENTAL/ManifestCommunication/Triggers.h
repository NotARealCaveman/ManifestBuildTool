#pragma once
#include <functional>
#include <EXPERIMENTAL/FunctionWrapper.h>
#include "Event.h"

namespace Manifest_Communication
{	
	typedef FunctionWrapper Trigger;

	template<typename ObservableGroup>
	struct ObservableGroupTriggers
	{
		static const Trigger loadTrigger;
		static const Trigger saveTrigger;
	};
	//hook up obersvable group actions to their triggers
	template<typename ObservableGroup>
	const Trigger ObservableGroupTriggers< ObservableGroup>::loadTrigger{ ObservableGroup::OnLoad };
	template<typename ObservableGroup>
	const Trigger ObservableGroupTriggers< ObservableGroup>::saveTrigger{ ObservableGroup::OnSave };
}