#include "GEX_skin.h"

using namespace Manifest_Parser;

DDL_Structure* GEX_Skin::Build(const std::string& partitionedStructure, DDL_ReferenceMap& referenceMap)
{
	auto result = New<DDL_Structure, ScratchPad<DDL_Structure>>(1);

	return result;
}