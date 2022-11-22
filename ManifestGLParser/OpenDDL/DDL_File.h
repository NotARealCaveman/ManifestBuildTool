#pragma once
#include <fstream>


#include "DDL_Structure.h"

namespace Manifest_Parser
{
	struct DDL_File
	{		
		DDL_ReferenceMap referenceMap;		
		std::vector<DDL_Structure> primaryStructures;
		std::fstream fileStream;
		uint64_t fileId;				
	};	
}