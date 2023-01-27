#pragma once
#include <ManifestParser/RegisteredGenerators.h>

#include "DDL_Buffers.h"

namespace Manifest_Parser
{
	struct DDL_Reference : public RegisteredBuilder
	{
		virtual DDL_Structure* Build(const std::string& partitionedStructure, DDL_ReferenceMap& referenceMap);

		ReferenceList referenceNames;
	};

	void ReserveReferenceContainers(DDL_Reference& reference); 
}