#pragma once
#include <ManifestGLParser/RegisteredGenerators.h>

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