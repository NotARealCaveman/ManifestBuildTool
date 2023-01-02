#pragma once
#include <string>
#include <map>

#include "ManifestGLParser/RegisteredBuilders.h"
#include <EXPERIMENTAL/Manifest_Allocator.h>

using namespace Manifest_Memory;

namespace Manifest_Parser
{	
	struct RegisteredGenerator
	{
		virtual DDL_Structure GenerateType(const std::string& structure, DDL_ReferenceMap& referenceMap) = 0;

		static std::map<std::string, RegisteredGenerator*> registeredGenerators;
	};

	template<typename T>
	struct Generator : public RegisteredGenerator
	{//generates a new type T in the type heap and invokes the builder callback		
		DDL_Structure GenerateType(const std::string& structure, DDL_ReferenceMap& referenceMap)
		{
			return DDL_Structure{ ((New<T, ScratchPad<T>>(1))->* & RegisteredBuilder::Build)(structure, referenceMap) };;
		};
	};	

	
}
