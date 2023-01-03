#pragma once
#include <vector>

#include "GEX_Node.h"

namespace Manifest_Parser
{
	struct GEX_BoneNode :public GEX_Node {  };//bone nodes are unique in use only

	struct GEX_BoneRefArray
	{
		//substructures
		ScratchPadVector<GEX_BoneNode*> boneNodeRefs;
	};

}