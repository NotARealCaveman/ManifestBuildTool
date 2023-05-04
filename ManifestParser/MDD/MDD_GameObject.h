#pragma once

#include <ManifestParser/OpenDDL/DDL_Primitives.h>

namespace Manifest_Parser
{	
	struct MDD_GameObject : public RegisteredBuilder
	{
		DDL_Structure* Build(const std::string_view& partitionedStructureView, DDL_ReferenceMap& referenceMap) final;
		//substructures
		//objcet indices are paired with each active bit in the type
		//ex index 1 = 2nd lowest active bit
		DDL_Int64 OBJECT_TYPE;//int used to differenitiate indices and type
		DDL_Uint64 objectIndices;//uint used ^ ^			^		^	^
	};
}