#pragma once
#include <vector>	
#include <atomic>

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

	template<typename Collider>
	struct BVH
	{
		MFfloat pos[3];
		Collider collider;
	};

	constexpr int Sphere = 0;
	constexpr float Cube = 1;

	//leaning on graphic resources reading from the database and the simulation writing to it.
	//simulation data changed much more drastically and in larger batches than graphic data.		
	//this will be supplied inside the simulation engine Rigidbodies::worldSpaces	

	struct Simulation
	{
		Xform xforms;
		BVH<decltype(Sphere)> spheres;
		BVH<decltype(Cube)> cubes;
	};

	struct SimulationSnapshot
	{
		Table<UniqueKey, Xform> xforms;//uses the uuid of the generated runtime object 
		Table<UniqueKey, BVH> xforms;//uses the uuid of the generated runtime object 
	};


	struct GeometryObjects
	{
		Table<PrimaryKey, GraphicID> geometryObjects;//pairs database mesh IDs to runtime ids

	};

	struct Material
	{
		GraphicID materialIDs[3];
		const GraphicID& operator[](const MFint32& index)
		{
			if (index < 0 || index > 2)
				return materialIDs[0];
			return materialIDs[index];
		}
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
		PrimaryKey* nodeMaterials;//matches table indices to the proper material entry
		
	};

	class ManifestRuntimeDatabase
	{
		public:								
			ManifestRuntimeDatabase(const ManifestBinaryDatabase& binaryDatabase);

			std::atomic<SimulationSnapshot*> simulationSnapshot;
			GeometryObjects geometryObjects;
			Materials materials;
			GeometryNodes geometryNodes;						
	};	
}