#include "Runtime_Database.h"

using namespace Manifest_Persistence;

ManifestRuntimeDatabase::ManifestRuntimeDatabase(const ManifestBinaryDatabase& binaryDatabase)
{	
	xoshiro256ss_state ss;
	//store number of nodes and create	
	geometryNodes.instancedNodeIDs.tableSize = binaryDatabase.binaryGeometryNodeTable.header.totalEntries;
	geometryNodes.instancedNodeIDs.keys = new UniqueKey[geometryNodes.instancedNodeIDs.tableSize];
	geometryNodes.instancedNodeIDs.values = new PrimaryKey[geometryNodes.instancedNodeIDs.tableSize];
	//create storage for node references
	geometryNodes.nodeGeometries = new PrimaryKey[geometryNodes.instancedNodeIDs.tableSize];
	geometryNodes.nodeMaterials = new PrimaryKey[geometryNodes.instancedNodeIDs.tableSize];
	//store number of materials and create
	materials.materials.tableSize = binaryDatabase.binaryMaterialTable.header.totalEntries;
	materials.materials.keys = new UniqueKey[materials.materials.tableSize];
	materials.materials.values = new Material[materials.materials.tableSize];
	//store number of geometries and create
	geometryObjects.geometryObjects.tableSize = binaryDatabase.binaryGeometryObjectTable.header.totalEntries;
	geometryObjects.geometryObjects.keys = new UniqueKey[geometryObjects.geometryObjects.tableSize];
	geometryObjects.geometryObjects.values = new GraphicID[geometryObjects.geometryObjects.tableSize];

	//load binary data into runtime
	auto nNodes = binaryDatabase.binaryGeometryNodeTable.header.totalEntries;
	auto creationIndexCounter{ 0 };	
	for (auto entry = 0; entry < nNodes; ++entry)
	{
		//get database node
		const auto& node = binaryDatabase.binaryGeometryNodeTable[entry];
		//add node into runtime
		auto runtimeID = ss.Crunch();
		DLOG(36, "Inserting node: " << geometryNodes.instancedNodeIDs.tableEntries << " with key " << node.header.nodeID <<" and value: " << runtimeID);
		geometryNodes.instancedNodeIDs.keys[geometryNodes.instancedNodeIDs.tableEntries] = node.header.nodeID;
		geometryNodes.instancedNodeIDs.values[geometryNodes.instancedNodeIDs.tableEntries] = runtimeID;//ss.Crunch();		
		//search for nodes geometry
		auto goBegin = geometryObjects.geometryObjects.begin<PrimaryKey>();
		auto goEnd = geometryObjects.geometryObjects.end<PrimaryKey>();
		auto instance = std::find(goBegin, goEnd, node.header.geometryID);		
		if (instance == goEnd)
		{
			//get node geometry
			const auto& geometry = binaryDatabase.binaryGeometryObjectTable[node.header.geometryID];
			//get node mesh
			const auto& mesh = binaryDatabase.binaryMeshTable[geometry.header.meshID];
			//add mesh to geometry runtime	
			instance = &(geometryObjects.geometryObjects.keys[geometryObjects.geometryObjects.tableEntries] = geometry.header.geometryID);
			geometryObjects.geometryObjects.values[geometryObjects.geometryObjects.tableEntries] = creationIndexCounter++;//glCreateVertexArray					
			geometryObjects.geometryObjects.tableEntries++;
		}
		//refer node to geometry
		geometryNodes.nodeGeometries[geometryNodes.instancedNodeIDs.tableEntries] = *instance;	
		//search for nodes materials
		auto mtlBegin = materials.materials.begin<PrimaryKey>();
		auto mtlEnd = materials.materials.end<PrimaryKey>();
		instance = std::find(mtlBegin, mtlEnd, node.header.materialID);
		if (instance == mtlEnd)
		{
			//get node material		
			const auto& material = binaryDatabase.binaryMaterialTable[node.header.materialID];
			instance = &(materials.materials.keys[materials.materials.tableEntries] = material.header.materialID);
			//add material to material runtime
			materials.materials.values[materials.materials.tableEntries].materialIDs[0] = material.header.diffuseID == KEY_NOT_PRESENT ? KEY_NOT_PRESENT : creationIndexCounter++;
			materials.materials.values[materials.materials.tableEntries].materialIDs[1] = material.header.noramlID == KEY_NOT_PRESENT ? KEY_NOT_PRESENT : creationIndexCounter++;
			materials.materials.values[materials.materials.tableEntries].materialIDs[2] = material.header.parallaxID == KEY_NOT_PRESENT ? KEY_NOT_PRESENT : creationIndexCounter++;
			materials.materials.tableEntries++;
		}
		//refer node to material
		geometryNodes.nodeMaterials[geometryNodes.instancedNodeIDs.tableEntries] = *instance;
		//account for the new node
		++geometryNodes.instancedNodeIDs.tableEntries;
	}
}

void Manifest_Persistence::SimThread(ManifestRuntimeDatabase& runtimeDatabase)
{	
	DLOG(35, "SimThread ID: " << std::this_thread::get_id());
	auto nPhysicsObjects = runtimeDatabase.geometryNodes.instancedNodeIDs.tableEntries;	nPhysicsObjects = 2000;
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
		//update sim frame
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
	auto nObjects = runtimeDatabase.geometryNodes.instancedNodeIDs.tableEntries;	nObjects = 2000;
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
		DLOG(35, "Render Frame: " << renderFrame << " simulation data: " << instancedVBOHandle[0].field[13]);	
		//update render frame
		renderFrame++;
		//sleep if permissible
		if (prediction > std::chrono::high_resolution_clock::now())
			std::this_thread::sleep_until(prediction);
	}
}