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

	//this reference system may require some "smart" tracking for multithreading/general persistence
	struct GeometryNodes
	{
		Table<UniqueKey, PrimaryKey> instancedNodeIDs;//pairs runtime ids to database nodes		
		PrimaryKey* nodeGeometries;//matches table indices to the proper geometryNode entry
		
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