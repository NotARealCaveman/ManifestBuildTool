#pragma once

#include "GEX_Bone.h"

namespace Manifest_Parser
{
	struct GEX_Skeleton
	{
		//substructures
		GEX_BoneRefArray boneRefArray;
		ScratchPadVector<GEX_Transform> boneBindPoses;
	};
}