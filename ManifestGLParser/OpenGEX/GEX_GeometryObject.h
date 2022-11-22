#pragma once

#include "GEX_Buffers.h"
#include "GEX_ObjectRef.h"
#include "GEX_Material.h"
#include "GEX_Mesh.h"
#include "GEX_Morph.h"

namespace Manifest_Parser
{
	/*The GeometryNode structure represents a single geometry node in the scene.Because it is a specific type of node, it possesses all of the characteristics of a generic node, such as an optional name,	transform, and animation.See the Node structure for more information.*/
	/*materialRefs: the index property of each material reference specifies to which part a mesh the material is applied by matching it with the material property of each IndexArray structure in the mesh*/
	struct GEX_GeometryNode : public GEX_Node
	{
		struct PropertyList {
			static constexpr int VISIBLE{ 0 };
			static constexpr int SHADOW{ VISIBLE + 1 };
			static constexpr int MOTIONBLUR{ SHADOW + 1 };
			static const std::map<std::string, uint32_t> typeProperties;
		};
		DDL_Structure Build(const std::string& partitionedStructure, DDL_ReferenceMap& referenceMap) final;

		//substructures
		GEX_ObjectRef objectRef;
		GEX_MaterialRef materialRef;
		std::vector<GEX_MaterialRef> materialRefs;//see comment
		std::vector<GEX_MorphWeight> morphWeights;				
		//properties - see GEX_GeometryObject
		bool* visible=nullptr;
		bool* shadow=nullptr;
		bool* motion_blur=nullptr;
	};

	/*The GeometryObject structure contains data for a geometry object. Multiple GeometryNode structures may reference a single geometry object, and this allows a scene to contain multiple instances of the same geometry with different transforms and materials.*/
	struct GEX_GeometryObject : public RegisteredBuilder
	{
		struct PropertyList {
			static constexpr int VISIBLE{ 0 };
			static constexpr int SHADOW{ VISIBLE + 1 };
			static constexpr int MOTIONBLUR{ SHADOW + 1 };
			static const std::map<std::string, uint32_t> typeProperties;
		};
		DDL_Structure Build(const std::string& partitionedStructure, DDL_ReferenceMap& referenceMap) final;

		//substructures
		GEX_Mesh mesh;
		GEX_Morph morph;
		//properties
		//the values of these properties may be overriden by a geometry node
		//properties of the object are used by the node when node supplied by the node
		bool visible = true;//if false,not rendered but potentially collidable
		bool shadow = true;//if false does not cast shadows
		bool motion_blur = true;//if false not rendered with motion blur
	};
}