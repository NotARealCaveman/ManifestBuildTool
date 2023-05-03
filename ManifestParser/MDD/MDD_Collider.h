#pragma once
#include <map>
#include <string>
#include <variant>

#include <ManifestParser/OpenDDL/DDL_Primitives.h>

namespace Manifest_Parser
{
	namespace COLLIDER_GEOMETRY
	{
		static constexpr MFu8 SPHERE{ 0 };
		static constexpr MFu8 CAPSULE{ 1 };
		static constexpr MFu8 CONVEXHULL{ 2 };
		static constexpr MFu8 DEFAULT_COLLIDER { SPHERE };
	}

	struct MDD_Collider : public RegisteredBuilder
	{
		struct PropertyList
		{
			static constexpr int SHAPE{ 0 };			
			static const std::map<std::string, DDL_BufferType> typeProperties;
		};
		DDL_Structure* Build(const std::string_view& partitionedStructureView, DDL_ReferenceMap& referenceMap) final;
		//substructures
		DDL_Float colliderData;
		//properties
		MFu8 colliderGeometry{ DEFAULT_COLLIDER };
	};
}