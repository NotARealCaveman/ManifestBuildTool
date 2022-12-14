#pragma once
#include <chrono>
#include <vector>	
#include <functional>

#include "Binary_Database.h"

#include<EXPERIMENTAL/EXPERIMENTAL_RUNTIME_DATA_STRUCTURES.h>
#include <EXPERIMENTAL/RNG.h>
#include <EXPERIMENTAL/Manifest_Allocator.h>

using namespace Manifest_Memory;
using namespace Manifest_Experimental;

namespace Manifest_Persistence
{
	typedef MFu32 GraphicID;	

	typedef Table<PrimaryKey, GraphicID> GeometryObjectTable;
	struct GeometryObjects
	{
		GeometryObjectTable geometryObjectTable;//pairs database mesh IDs to runtime ids
	};

	struct Material
	{
		GraphicID materialIDs[3];//[0]:diff,[1]:nrml,[2]:parallax
		inline const GraphicID& operator[](const MFint32& index) { return materialIDs[index]; };
	};

	typedef Table<PrimaryKey, Material> MaterialTable;
	struct Materials
	{
		MaterialTable materialTable;//pairs database mtls to runtime ids
	};

	//this reference system may require some "smart" tracking for multithreading/general persistence
	typedef Table<PrimaryKey, UniqueKey> GeometryNodeTable;
	struct GeometryNodes
	{
		GeometryNodeTable geometryNodeTable;//pairs runtime ids to database nodes		
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
		MFu64* xformTable{};
	};
	MFu64* Simulate(const Simulation& simulation, const MFsize nBodies);


	enum class DatabaseInitializerTypes
	{
		STATE_INITIALIZER
	};

	//Currently exploring a push/pull paradigm for updating and centralizing shared game state in the runtime database
	class ManifestRuntimeDatabase
	{
		private:	
			//if present - new data to be pulled
			std::atomic<MFu64*> newStates{ nullptr };
			
			GeometryNodes geometryNodes;
			GeometryObjects geometryObjects;
			Materials materials;

			SimulationSnapshot committedSimulation;

			SRSWExchangeLock stateLock;//R-W opperations on states

		public:
			ManifestRuntimeDatabase(const ManifestBinaryDatabase& binaryDatabase);	 
			void INITIALIZE_FIRST_STORES__BYPASS_PULL_BRANCH();
			//atomically pushes a new simulation states to database and cleans up unused state memory if needed
			void PushStates(MFu64* stateSnapshot);
			//returns currently committed simulation state - if a new state has been pushed, atomically pulls update and cleans up previous state memory 
			MFu64* PullStates();

			void PushGeometryNodes();
			GeometryNodes* PullGeometryNodes();

			void PushGeometryObjects();
			GeometryObjects* PullGeometryObjects();

			void PushMaterials();
			Materials* PullMaterials();			

			std::atomic_flag init = ATOMIC_FLAG_INIT;		

			std::thread::id simThreadId;
			std::thread::id renderThreadId;
	};	

	void SimThread(ManifestRuntimeDatabase& runtimeDatabase);
	void RenderThread(ManifestRuntimeDatabase& runtimeDatabase);	
}

