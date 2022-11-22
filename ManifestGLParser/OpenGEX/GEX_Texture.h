#pragma once

#include "GEX_Color.h"
#include "GEX_Transform.h"

namespace Manifest_Parser
{
	struct GEX_Spectrum : public RegisteredBuilder
	{
		struct PropertyList {

			static constexpr int ATTRIB{ 0 };
			static constexpr int MIN{ ATTRIB +1 };
			static constexpr int MAX{ MIN + 1 };
			static const std::map<std::string, uint32_t> typeProperties;
		};
		DDL_Structure Build(const std::string& partitionedStructure, DDL_ReferenceMap& referenceMap) final;

		//substructures
		float distribution;
		//properties
		std::string attrib;
		uint32_t min;
		uint32_t max;
	};

	struct GEX_Texture : public RegisteredBuilder
	{
		struct PropertyList {
			static constexpr int ATTRIB{ 0 };
			static constexpr int TEXCOORD{ ATTRIB+1 };
			static constexpr int SWIZZLE{ TEXCOORD+1 };
			static constexpr int XADDRESS{ SWIZZLE +1 };
			static constexpr int YADDRESS{ XADDRESS +1 };
			static constexpr int ZADDRESS{ YADDRESS +1 };
			static constexpr int BORDER{ ZADDRESS +1 };
			static const std::map<std::string, uint32_t> typeProperties;
		};
		DDL_Structure Build(const std::string& partitionedStructure, DDL_ReferenceMap& referenceMap)final;

		//substructures
		std::string fileName;
		GEX_Transform* transform;
		GEX_Translation* translation;
		GEX_Rotation* rotation;
		GEX_Scale scale;
		//properties
		std::string attrib;
		//specifies which texture coordinate set belonging to a mesh should be used to access the texture
		uint32_t texcoord = 0;
		std::string swizzle = "i";
		std::string x_address = "repeat";
		std::string y_address = "repeat";
		std::string z_address = "repeat";
		std::string border = "zero";
	};
}
