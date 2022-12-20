#pragma once
#include <chrono>
#include <vector>	
#include <functional>

#include "Binary_Database.h"

#include<EXPERIMENTAL/EXPERIMENTAL_RUNTIME_DATA_STRUCTURES.h>
#include <EXPERIMENTAL/RNG.h>

namespace Manifest_Persistence
{
	typedef MFu32 GraphicID;	

	struct GeometryObjects
	{
		Table<PrimaryKey, GraphicID> geometryObjects;//pairs database mesh IDs to runtime ids
	};

	struct Material
	{
		GraphicID materialIDs[3];
		inline const GraphicID& operator[](const MFint32& index) { return materialIDs[index]; };
	};

	struct Materials
	{
		Table<PrimaryKey, Material> materials;//pairs database mtls to runtime ids
	};

	//this reference system may require some "smart" tracking for multithreading/general persistence
	struct GeometryNodes
	{
		Table<PrimaryKey, UniqueKey> instancedNodeIDs;//pairs runtime ids to database nodes		
		PrimaryKey* nodeGeometries;//matches table indices to the proper geometryNode entry
		PrimaryKey* nodeMaterials;//matches table indices to the proper material entry
		
	};	

	struct Xform { float field[16]; };

	struct Bodies
	{
		UniqueKey* bodyID;
		Xform* worldSpaces;		
	};

	typedef Table<UniqueKey, Xform> XformTable;

	struct Simulation
	{		
		Bodies bodies;
		MFu64 simulationFrame{ 0 };
	};

	//acts as a double buffer for the simulation to store shared data of a complete simulation frame 
	struct SimulationSnapshot
	{
		MFu64 simulationFrame;
		XformTable xformTable;
	};

	//inserting into the database should be designed to be obstruction free where possible
	//only the rendering thread should be inserting/removing graphic related entries from the db
	//only the mt/sim thread should be updating the simulation data pointer
	//synchronization should only occur when data will be interacted with both threads during runtime
	class ManifestRuntimeDatabase
	{
		private:				
		public:								
			ManifestRuntimeDatabase(const ManifestBinaryDatabase& binaryDatabase);			
			
			//if present - new snapshot
			std::atomic<SimulationSnapshot*> simulationSnapshot;
			GeometryNodes geometryNodes;
			GeometryObjects geometryObjects;
			Materials materials;

			std::atomic_flag init = ATOMIC_FLAG_INIT;
			SRSWExchangeLock simulationLock;
			ExchangeLock printLock;			
	};	

	void SimThread(ManifestRuntimeDatabase& runtimeDatabase);
	void RenderThread(ManifestRuntimeDatabase& runtimeDatabase);	
}

