#pragma once
#include <string>

#include <ManifestParser/RegisteredGenerators.h>
#include <ManifestParser/OpenDDL_Parser.h>
#include <ManifestParser/OpenDDL/DDL_Primitives.h>

namespace Manifest_Parser
{
	enum class GEX_TransformType
	{
		transform4x4, transform3x4, //16 entire matrix ,12 homogenous w transform
		transform3x3,				//9, upper 3x3
		transform2x3, transform2x2	//6 2x2 + w translation,4 2x2 + no w translation
	};
	static constexpr size_t TransformSize = 16;
	struct GEX_Transform : public RegisteredBuilder
	{
		struct PropertyList {
			static constexpr int OBJECT{ 0 };
			static constexpr int KIND{ OBJECT + 1 };
			static const std::map<std::string, uint32_t> typeProperties;
		};
		DDL_Structure* Build(const std::string& partitionedStructure, DDL_ReferenceMap& referenceMap) final;
		//substructures		
		DDL_Float field;
		//properties
		bool object = false;		
		ScratchPadString kind;
	};
	//checks current trasnform type if not a 4x4 allocates new memory and builds proper 4x4 matrix in GEX_Transform::field - the old field memory is released by the function and the newly allocated memory is pointed to by GEX_Transform::field
	void FormatTransform(const size_t& transformType, float** unformattedData);

	typedef GEX_Transform GEX_Rotation;
	typedef GEX_Rotation GEX_Translation;
	typedef GEX_Translation GEX_Scale;//repurpose XYZ from translation to diagonal
}