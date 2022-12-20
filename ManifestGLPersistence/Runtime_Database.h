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

	//Currently exploring a push/pull paradigm for updating and centralizing shared game state in the runtime database
	class ManifestRuntimeDatabase
	{
		private:	
			//if present - new snapshot
			std::atomic<SimulationSnapshot*> newSimulation{ nullptr };			
		public:								
			ManifestRuntimeDatabase(const ManifestBinaryDatabase& binaryDatabase);	
			//atomically pushes a new simulation to the database and cleans up unused simulation memory if present
			void PushSimulation(SimulationSnapshot* snapshot)
			{
				SimulationSnapshot* prevSim = nullptr;
				//atomically check for old sim and store new
				simulationLock.Write(std::function([&]()
					{
						prevSim = newSimulation.load(std::memory_order_acquire);
						newSimulation.store(snapshot, std::memory_order_release);
					}));
				//if previous sim found delete - renderer missed sim
				if (prevSim)
				{
					delete[] prevSim->xformTable.keys;
					delete[] prevSim->xformTable.values;
					delete prevSim;
					prevSim = nullptr;
				}
			}
			//returns the currently committed simulation - if a new simulation has been pushed, atomically pulls update and cleans up previous simulation memory 
			SimulationSnapshot* PullSimulation()
			{
				//return committed if nothing new present
				if (!newSimulation.load(std::memory_order_acquire))
					return committedSimulation;

				//new simulation present - store old simulation
				auto prevSim = committedSimulation;
				//atomically updates simulation and removes flag
				simulationLock.Read(std::function([&]()
					{
						committedSimulation = newSimulation.load(std::memory_order_acquire);//upate simulation	
						newSimulation.store(nullptr, std::memory_order_release);//remove flag
					}));
				//relase old memory if applicable 				
				if (prevSim)
				{//may explore option that requires sim to render
					delete[] prevSim->xformTable.keys;
					delete[] prevSim->xformTable.values;
					delete prevSim;
				}

				return committedSimulation;
			}			
			SimulationSnapshot* committedSimulation{ nullptr };
			GeometryNodes geometryNodes;
			GeometryObjects geometryObjects;
			Materials materials;			

			std::atomic_flag init = ATOMIC_FLAG_INIT;
			SRSWExchangeLock simulationLock;			
	};	

	void SimThread(ManifestRuntimeDatabase& runtimeDatabase);
	void RenderThread(ManifestRuntimeDatabase& runtimeDatabase);	
}

