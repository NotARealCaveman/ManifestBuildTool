#pragma once
#include <vector>	
#include <atomic>
#include <chrono>

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

	struct SimulationSnapshot
	{
		std::atomic<MFu64> simulationFrame{ 0 };
		XformTable xformTable;
	};

	//inserting into the database should be designed to be obstruction free where possible
	//only the rendering thread should be inserting/removing graphic related entries from the db
	//only the mt/sim thread should be updating the simulation data pointer
	//synchronization should only occur when data will be interacted with both threads during runtime

	//loop on exchange while lock is held(1) until released(0)
	constexpr MFu8 LOCKED{ 1 };
	constexpr MFu8 UNLOCKED{ 0 };
	struct ExchangeLock
	{
		std::atomic<MFu8> lock{ UNLOCKED };
		void Lock() { while (lock.exchange(LOCKED, std::memory_order_acquire)); };
		void Unlock() { lock.store(UNLOCKED, std::memory_order_release); };
	};	

	class ManifestRuntimeDatabase
	{
		private:				
		public:								
			ManifestRuntimeDatabase(const ManifestBinaryDatabase& binaryDatabase);			
			
			SimulationSnapshot simulationSnapshot;
			GeometryNodes geometryNodes;
			GeometryObjects geometryObjects;
			Materials materials;

			std::atomic_flag init = ATOMIC_FLAG_INIT;
			ExchangeLock simulationLock;			
	};	

	void SimThread(ManifestRuntimeDatabase& runtimeDatabase);
	void RenderThread(ManifestRuntimeDatabase& runtimeDatabase);
}

