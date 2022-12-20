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
		XformTable* xformTable{nullptr};
	};


	//Currently exploring a push/pull paradigm for updating and centralizing shared game state in the runtime database
	class ManifestRuntimeDatabase
	{
		private:	
			//if present - new snapshot					
			std::atomic<XformTable*> newStates{ nullptr };
			SRSWExchangeLock stateLock;//R-W opperations on states
		public:
			ManifestRuntimeDatabase(const ManifestBinaryDatabase& binaryDatabase);	

			//atomically pushes a new simulation states to database and cleans up unused state memory if needed
			void PushStates(XformTable* stateSnapshot);
			//returns currently committed simulation state - if a new state has been pushed, atomically pulls update and cleans up previous state memory 
			XformTable* PullStates();

			void PushGeometryNodes();
			GeometryNodeTable* PullGeometryNodes();

			SimulationSnapshot committedSimulation;
			GeometryNodes geometryNodes;
			GeometryObjects geometryObjects;
			Materials materials;			

			std::atomic_flag init = ATOMIC_FLAG_INIT;			
	};	

	void SimThread(ManifestRuntimeDatabase& runtimeDatabase);
	void RenderThread(ManifestRuntimeDatabase& runtimeDatabase);	
}

