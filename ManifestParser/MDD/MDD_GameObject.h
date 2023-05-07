#pragma once

#include <ManifestDefinitions/GameObject_Definitions.h>
#include <ManifestParser/OpenDDL/DDL_Primitives.h>
#include <ManifestParser/OpenDDL/DDL_Reference.h>
#include <ManifestParser/OpenGEX/GEX_GeometryObject.h>


namespace Manifest_Parser
{	
	//A game object is a high level abstraction over specific game object types
	//each game object supplies an object ID which is fed to the primary custodian of that index for the data pipeline.
	//EX: Rigidbody object is build from a physics and geometry node thus contains references to the two structures, it contains an object ID of 1.this object ID would be fed to the rigid body and collider during the build process and fill out the id fields
	struct MDD_GameObject : public RegisteredBuilder
	{
		DDL_Structure* Build(const std::string_view& partitionedStructureView, DDL_ReferenceMap& referenceMap) final;
		//substructures
		//references are paired with each active bit in the type
		//ex index 1 = 2nd lowest active bit
		DDL_Reference objectReferences;
		DDL_Int64 OBJECT_TYPE;
		DDL_Uint64 objectID;		
	};
}