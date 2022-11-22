#pragma once
#include <string>

#include <ManifestGLParser/OpenDDL_Parser.h>
#include <ManifestGLParser/OpenDDL/DDL_Reference.h>


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
		DDL_Structure Build(const std::string& partitionedStructure, DDL_ReferenceMap& referenceMap) final;

		//substructures
		GEX_ObjectType type;
	};
}