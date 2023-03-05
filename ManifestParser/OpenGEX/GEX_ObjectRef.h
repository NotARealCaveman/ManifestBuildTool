#pragma once
#include <string>

#include <ManifestParser/OpenDDL_Parser.h>
#include <ManifestParser/OpenDDL/DDL_Reference.h>


namespace Manifest_Parser
{
	enum class GEX_ObjectType
	{
		GeometryObject,
		CameraObject,
		LightObject
	};

	struct GEX_ObjectRef : public DDL_Reference
	{
		DDL_Structure* Build(const std::string_view& partitionedStructureView, DDL_ReferenceMap& referenceMap) final;

		//substructures
		GEX_ObjectType type;
	};
}