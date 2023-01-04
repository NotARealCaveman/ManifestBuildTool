#pragma once
#include <algorithm>

#include <ManifestGLParser/OpenDDL/DDL_Primitives.h>

#include "GEX_Skin.h"

namespace Manifest_Parser
{
	/*All VertexArray structures belonging to a mesh must specify data for the same number of vertices.*/
	struct GEX_VertexArray : public RegisteredBuilder
	{
		struct PropertyList {
			static constexpr int ATTRIB{ 0 };
			static constexpr int INDEX{ ATTRIB + 1 };
			static constexpr int MORPH{ INDEX + 1 };
			static const std::map<std::string, uint32_t> typeProperties;
		};
		DDL_Structure* Build(const std::string& partitionedStructure, DDL_ReferenceMap& referenceMap) final;
		//substructures
		DDL_Float vertexArray;
		//properties
		ScratchPadString attrib;
		uint32_t index = 0;
		uint32_t morph = 0;
		//database extension
		uint8_t bufferIndex;
	};
	uint8_t GetBufferIndex(const std::string& bufferAttrib);

	/*Each IndexArray structure specifies how the vertices are assembled into geometric primitives.For lines, triangles, and quads, the index array contains subarrays that each specify the indexes of the	vertices composing a single primitive.For points, line strips, and triangle strips, the index data is stored as a single array.*/
	struct GEX_IndexArray : public RegisteredBuilder
	{
		struct PropertyList {
			static constexpr int MATERIAL{ 0 };
			static constexpr int RESTART{ MATERIAL + 1 };
			static constexpr int FRONT{ RESTART + 1 };
			static const std::map<std::string, uint32_t> typeProperties;
		};
		DDL_Structure* Build(const std::string& partitionedStructure, DDL_ReferenceMap& referenceMap) final;

		//substructures
		DDL_Int32 indexArray;
		//properties
		uint32_t material = 0;
		uint64_t restart;
		ScratchPadString front = "ccw";
	};

	/*The Mesh structure contains data for a single geometric mesh, and a GeometryObject structure contains one mesh for each level of detail. A mesh may also contain a single Skin structure that holds the skeleton and bone influence data needed for skinning.*/	
	/*Note: If a mesh does not contain an IndexArray structure, then it is as if an index array with the default properties existedand contained each index between 0 and n −1 in order and grouped into subarrays as	necessary, where n is the number of vertices in each VertexArray structure.*/
	struct GEX_Mesh : public RegisteredBuilder
	{
		struct PropertyList{
			static constexpr int LOD{ 0 };
			static constexpr int PRIMTIIVE{ LOD + 1 };
			static const std::map<std::string, uint32_t> typeProperties;
		};				
		DDL_Structure* Build(const std::string& partitionedStructure, DDL_ReferenceMap& referenceMap) final;

		//substructures
		ScratchPadVector<GEX_VertexArray> vertexArrays;//see comment
		ScratchPadVector<GEX_IndexArray> indexArrays;//see comment
		GEX_Skin* skin;
		//properties
		uint32_t lod = 0;//starting from 0, specifies a progressively lower resolution mesh
		ScratchPadString primitive = "triangles";//specifies how the vertices are to be drawn
	};
}