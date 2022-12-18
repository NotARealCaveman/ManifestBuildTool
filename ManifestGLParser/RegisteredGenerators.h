#pragma once
#include <string>
#include <map>

#include "ManifestGLParser/RegisteredBuilders.h"

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
			T* typeHeap;
			DDL_Structure result{((typeHeap = new T)->*&RegisteredBuilder::Build)(structure,referenceMap)};
			result.typeHeap = typeHeap;

			return result;
		};
	};	

	
}
