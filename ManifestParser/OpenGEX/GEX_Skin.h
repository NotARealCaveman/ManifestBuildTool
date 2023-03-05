#pragma once

#include <ManifestParser/OpenDDL/DDL_Primitives.h>

#include "GEX_Transform.h"
#include "GEX_Skeleton.h"

namespace Manifest_Parser
{
	typedef DDL_Buffer GEX_BoneCountArray;
	typedef DDL_Buffer GEX_BoneIndexArray;
	typedef DDL_Buffer GEX_BoneWeightArray;

	struct GEX_Skin : public RegisteredBuilder
	{
		DDL_Structure* Build(const std::string_view& partitionedStructureView, DDL_ReferenceMap& referenceMap) final;

		//substructures
		GEX_Transform* meshBindPose;
		GEX_Skeleton skeleton;
		GEX_BoneCountArray boneCountArray;
		GEX_BoneIndexArray boneIndexArray;
		GEX_BoneWeightArray boneWeightArray;
	};
}