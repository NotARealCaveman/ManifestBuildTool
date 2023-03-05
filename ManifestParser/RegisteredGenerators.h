#pragma once
#include <string>
#include <string_view>
#include <map>

#include "ManifestParser/RegisteredBuilders.h"
#include <ManifestMemory/Manifest_Allocator.h>

using namespace Manifest_Memory;

namespace Manifest_Parser
{	
	struct RegisteredGenerator
	{
		virtual DDL_Structure* GenerateType(const std::string_view& structureView, DDL_ReferenceMap& referenceMap) = 0;

		static std::map<std::string, RegisteredGenerator*> registeredGenerators;
	};

	template<typename T>
	struct Generator : public RegisteredGenerator
	{//generates a new type T in the type heap and invokes the builder callback		
		DDL_Structure* GenerateType(const std::string_view& structureView, DDL_ReferenceMap& referenceMap)
		{						
			DISABLE
			{
				T* newT = New<T, ScratchPad<T>>(1);
			auto result = newT->Build(structureView, referenceMap);
			return result;
			}
			
			return (New<T, ScratchPad<T>>(1)->*&RegisteredBuilder::Build)(structureView, referenceMap);
		};
	};	

	
}
