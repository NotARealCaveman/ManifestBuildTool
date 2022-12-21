#include "Runtime_Database.h"

using namespace Manifest_Persistence;

ManifestRuntimeDatabase::ManifestRuntimeDatabase(const ManifestBinaryDatabase& binaryDatabase)
{	
	xoshiro256ss_state ss;
	//store number of nodes and create	
	geometryNodes.geometryNodeTable.tableSize = binaryDatabase.binaryGeometryNodeTable.header.totalEntries;
	geometryNodes.geometryNodeTable.keys = new UniqueKey[geometryNodes.geometryNodeTable.tableSize];
	geometryNodes.geometryNodeTable.values = new PrimaryKey[geometryNodes.geometryNodeTable.tableSize];
	//create storage for node references
	geometryNodes.nodeGeometries = new PrimaryKey[geometryNodes.geometryNodeTable.tableSize];
	geometryNodes.nodeMaterials = new PrimaryKey[geometryNodes.geometryNodeTable.tableSize];
	//store number of materials and create
	materials.materialTable.tableSize = binaryDatabase.binaryMaterialTable.header.totalEntries;
	materials.materialTable.keys = new UniqueKey[materials.materialTable.tableSize];
	materials.materialTable.values = new Material[materials.materialTable.tableSize];
	//store number of geometries and create
	geometryObjects.geometryObjectTable.tableSize = binaryDatabase.binaryGeometryObjectTable.header.totalEntries;
	geometryObjects.geometryObjectTable.keys = new UniqueKey[geometryObjects.geometryObjectTable.tableSize];
	geometryObjects.geometryObjectTable.values = new GraphicID[geometryObjects.geometryObjectTable.tableSize];

	//load binary data into runtime
	auto nNodes = binaryDatabase.binaryGeometryNodeTable.header.totalEntries;
	auto creationIndexCounter{ 0 };	
	for (auto entry = 0; entry < nNodes; ++entry)
	{
		//get database node
		const auto& node = binaryDatabase.binaryGeometryNodeTable[entry];
		//add node into runtime
		auto runtimeID = ss.Crunch();
		DLOG(36, "Inserting node: " << geometryNodes.geometryNodeTable.tableEntries << " with key " << node.header.nodeID <<" and value: " << runtimeID);
		geometryNodes.geometryNodeTable.keys[geometryNodes.geometryNodeTable.tableEntries] = node.header.nodeID;
		geometryNodes.geometryNodeTable.values[geometryNodes.geometryNodeTable.tableEntries] = runtimeID;//ss.Crunch();		
		//search for nodes geometry
		auto goBegin = geometryObjects.geometryObjectTable.begin<PrimaryKey>();
		auto goEnd = geometryObjects.geometryObjectTable.end<PrimaryKey>();
		auto instance = std::find(goBegin, goEnd, node.header.geometryID);		
		if (instance == goEnd)
		{
			//get node geometry
			const auto& geometry = binaryDatabase.binaryGeometryObjectTable[node.header.geometryID];
			//get node mesh
			const auto& mesh = binaryDatabase.binaryMeshTable[geometry.header.meshID];
			//add mesh to geometry runtime	
			instance = &(geometryObjects.geometryObjectTable.keys[geometryObjects.geometryObjectTable.tableEntries] = geometry.header.geometryID);
			geometryObjects.geometryObjectTable.values[geometryObjects.geometryObjectTable.tableEntries] = creationIndexCounter++;//glCreateVertexArray					
			geometryObjects.geometryObjectTable.tableEntries++;
		}
		//refer node to geometry
		geometryNodes.nodeGeometries[geometryNodes.geometryNodeTable.tableEntries] = *instance;	
		//search for nodes materials
		auto mtlBegin = materials.materialTable.begin<PrimaryKey>();
		auto mtlEnd = materials.materialTable.end<PrimaryKey>();
		instance = std::find(mtlBegin, mtlEnd, node.header.materialID);
		if (instance == mtlEnd)
		{
			//get node material		
			const auto& material = binaryDatabase.binaryMaterialTable[node.header.materialID];
			instance = &(materials.materialTable.keys[materials.materialTable.tableEntries] = material.header.materialID);
			//add material to material runtime
			materials.materialTable.values[materials.materialTable.tableEntries].materialIDs[0] = material.header.diffuseID == KEY_NOT_PRESENT ? KEY_NOT_PRESENT : creationIndexCounter++;
			materials.materialTable.values[materials.materialTable.tableEntries].materialIDs[1] = material.header.noramlID == KEY_NOT_PRESENT ? KEY_NOT_PRESENT : creationIndexCounter++;
			materials.materialTable.values[materials.materialTable.tableEntries].materialIDs[2] = material.header.parallaxID == KEY_NOT_PRESENT ? KEY_NOT_PRESENT : creationIndexCounter++;
			materials.materialTable.tableEntries++;
		}
		//refer node to material
		geometryNodes.nodeMaterials[geometryNodes.geometryNodeTable.tableEntries] = *instance;
		//account for the new node
		++geometryNodes.geometryNodeTable.tableEntries;
	}
}

void ManifestRuntimeDatabase::PushStates(XformTable* stateSnapshot)
{
	XformTable* prevStates{ nullptr };
	//atomically check for old sim and store new
	stateLock.Write(std::function([&]()
		{
			prevStates = newStates.load(std::memory_order_acquire);
			newStates.store(stateSnapshot, std::memory_order_release);
		}));
	//if previous sim found delete - renderer missed sim
	if (prevStates)
	{
		delete[] prevStates->keys;
		delete[] prevStates->values;
		delete prevStates;
		prevStates = nullptr;
	}
}

XformTable* ManifestRuntimeDatabase::PullStates()
{
	//return committed if nothing new present
	if (!newStates.load(std::memory_order_acquire))
		return committedSimulation.xformTable;

	//new simulation present - store old simulation
	XformTable* prevStates{ nullptr };
	//atomically updates simulation and removes flag
	stateLock.Read(std::function([&]()
		{
			prevStates = committedSimulation.xformTable;
			committedSimulation.xformTable = newStates.load(std::memory_order_acquire);//upate simulation	
			newStates.store(nullptr, std::memory_order_release);//remove flag
		}));
	//relase old memory if applicable 				
	if (prevStates)
	{//may explore option that requires sim to render
		delete[] prevStates->keys;
		delete[] prevStates->values;
		delete prevStates;
	}

	return committedSimulation.xformTable;
}

GeometryNodes* ManifestRuntimeDatabase::PullGeometryNodes()
{
	return &geometryNodes;
};
GeometryObjects* ManifestRuntimeDatabase::PullGeometryObjects()
{
	return &geometryObjects;
};
Materials* ManifestRuntimeDatabase::PullMaterials()
{
	return &materials;
};

void Manifest_Persistence::SimThread(ManifestRuntimeDatabase& runtimeDatabase)
{	
	DLOG(35, "SimThread ID: " << std::this_thread::get_id());
	auto nPhysicsObjects = runtimeDatabase.PullGeometryNodes()->geometryNodeTable.tableEntries;
	Simulation simulation;	
	simulation.bodies.bodyID = new UniqueKey[nPhysicsObjects];
	simulation.bodies.worldSpaces = new Xform[nPhysicsObjects];
	memset(simulation.bodies.worldSpaces, 0, sizeof(Xform) * nPhysicsObjects);	
	runtimeDatabase.init.test_and_set();
	runtimeDatabase.init.notify_one();
	//sleep and predicition
	auto simInterval = std::chrono::duration<double>{ 1/60.0 };
	auto begin = std::chrono::high_resolution_clock::now();	
	for(;;)
	{			
		auto prediction = begin + simulation.simulationFrame * simInterval;						
		for (auto object{ 0 }; object < nPhysicsObjects; ++object)			simulation.bodies.worldSpaces[object].field[13] = simulation.bodies.worldSpaces[object].field[13] + 1;
		//copy results to snapshot
		XformTable* stateSnapshot{ new XformTable};	
		stateSnapshot->tableEntries = stateSnapshot->tableSize = nPhysicsObjects;
		stateSnapshot->keys = new UniqueKey[nPhysicsObjects];
		stateSnapshot->values = new Xform[nPhysicsObjects];
		memcpy(stateSnapshot->values, simulation.bodies.worldSpaces, sizeof(Xform) * nPhysicsObjects);
		//sync end simulation results - handles memory cleanup	
		runtimeDatabase.PushStates(stateSnapshot);	
		//continue sim and push updates
		//update sim frame - concludes simulation for frame
		simulation.simulationFrame++;				
		//sleep if permissible
		if (prediction > std::chrono::high_resolution_clock::now())
			std::this_thread::sleep_until(prediction);
	}
}

void Manifest_Persistence::RenderThread(ManifestRuntimeDatabase& runtimeDatabase)
{
	DLOG(35, "RenderThread ID : " << std::this_thread::get_id());
	auto set = false;
	if (!(set = runtimeDatabase.init.test()))
		runtimeDatabase.init.wait(set, std::memory_order_relaxed);
	MFu64 renderFrame = 0;	
	//represents the handle to the graphic resource which works with the world space data
	auto nObjects = runtimeDatabase.PullGeometryNodes()->geometryNodeTable.tableEntries;
	Xform* instancedVBOHandle = new Xform[nObjects];
	//sleep and predicition
	auto frameInterval = std::chrono::duration<double>{ 1 / 144.0 };
	auto begin = std::chrono::high_resolution_clock::now();	
	XformTable* stateSnapshot{nullptr};
	for (;;)
	{
		auto prediction = begin + renderFrame * frameInterval;	
		auto currentStates = stateSnapshot;
		//get current simulation data - update vbo if new
		if (currentStates != (stateSnapshot = runtimeDatabase.PullStates()))
			memcpy(instancedVBOHandle, stateSnapshot->values, sizeof(Xform) * nObjects);		
		//DLOG(35, "Render Frame: " << renderFrame << " simulation data: " << instancedVBOHandle[0].field[13]);
		//update render frame
		renderFrame++;
		//sleep if permissible
		if (prediction > std::chrono::high_resolution_clock::now())
			std::this_thread::sleep_until(prediction);
	}
}