#pragma once
#include <vector>	

#include "Binary_Database.h"

#include<EXPERIMENTAL/EXPERIMENTAL_RUNTIME_DATA_STRUCTURES.h>
#include <EXPERIMENTAL/RNG.h>



namespace Manifest_Persistence
{
	typedef MFu32 GraphicID;	

	struct Xform
	{
		MFfloat data[16];
	};

	//leaning on graphic resources reading from the database and the simulation writing to it.
	//simulation data changed much more drastically and in larger batches than graphic data.		
	//this will be supplied inside the simulation engine Rigidbodies::worldSpaces	
	struct WorldSpaces
	{
		Table<UniqueKey, Xform> xforms;//uses the uuid of the generated runtime object 
	};


	struct GeometryObjects
	{
		Table<PrimaryKey, GraphicID> geometryObjects;//pairs database mesh IDs to runtime ids

	};

	struct Material
	{
		GraphicID materialIDs[3];
	};

	struct Materials
	{
		Table<PrimaryKey, Material> materials;//pairs database mtls to runtime ids
	};

	struct GeometryNodes
	{
		Table<UniqueKey, PrimaryKey> instancedNodeIDs;//pairs runtime ids to database nodes		
		Table<UniqueKey, GeometryObjects*> geoemtryRefs;//points to nodes geometry
		
	};

	class ManifestRuntimeDatabase
	{
		public:								
			ManifestRuntimeDatabase(const ManifestBinaryDatabase& binaryDatabase, const WorldSpaces& worldSpaces);

			GeometryObjects geometryObjects;
			Materials materials;
			GeometryNodes geometryNodes;						
	};	
}