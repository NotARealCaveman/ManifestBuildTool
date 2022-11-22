#pragma once
#include <vector>

#include "GEX_Buffers.h"
#include "GEX_Name.h"
#include "GEX_Transform.h"

namespace Manifest_Parser
{
	//forward declares
	struct GEX_Animation;
	struct GEX_BoneNode;
	struct GEX_GeometryNode;
	struct GEX_CameraNode;
	struct GEX_LightNode;

	//a GEX_Node is a single generic node in a scene with no associated object
	struct GEX_Node : public RegisteredBuilder
	{
		//GEX_Node build DOES NOT further partition - it takes in only substructures
		virtual DDL_Structure Build(const std::string& partitionedStructure, DDL_ReferenceMap& referenceMap);

		//substructures
		GEX_Name name;
		std::vector<GEX_Transform> transforms;
		std::vector<GEX_Animation*> animations;
		std::vector<GEX_BoneNode*> boneNode;
		std::vector<GEX_GeometryNode*> geometryNode;
		std::vector<GEX_CameraNode*> cameraNode;
		std::vector<GEX_LightNode*> lightNode;
	};

	void ReserveNodeContainers(GEX_Node& node);

}