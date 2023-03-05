#pragma once
#include <fstream>

#include "DDL_Structure.h"

namespace Manifest_Parser
{
	struct DDL_File
	{		
		DDL_ReferenceMap referenceMap;		
		ScratchPadVector<DDL_Structure*> primaryStructures;		
		uint64_t fileId;				
	};	
}