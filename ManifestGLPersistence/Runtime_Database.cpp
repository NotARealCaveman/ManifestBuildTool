#include "Runtime_Database.h"

using namespace Manifest_Persistence;

ManifestRuntimeDatabase::ManifestRuntimeDatabase(const ManifestBinaryDatabase& binaryDatabase)
{
	INITIALIZE_FIRST_STORES__BYPASS_PULL_BRANCH();

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
		//DLOG(36, "Inserting node: " << geometryNodes.geometryNodeTable.tableEntries << " with key " << node.header.nodeID <<" and value: " << runtimeID);
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
			materials.materialTable.values[materials.materialTable.tableEntries].materialIDs[1] = material.header.normalID == KEY_NOT_PRESENT ? KEY_NOT_PRESENT : creationIndexCounter++;
			materials.materialTable.values[materials.materialTable.tableEntries].materialIDs[2] = material.header.parallaxID == KEY_NOT_PRESENT ? KEY_NOT_PRESENT : creationIndexCounter++;
			materials.materialTable.tableEntries++;
		}
		//refer node to material
		geometryNodes.nodeMaterials[geometryNodes.geometryNodeTable.tableEntries] = *instance;
		//account for the new node
		++geometryNodes.geometryNodeTable.tableEntries;
	}
}

void ManifestRuntimeDatabase::PushStates(MFu64* stateSnapshot)
{
	MFu64* prevStates{ nullptr };
	
	//atomically swap old  and  new store sim
	stateLock.Write(std::function([&]()
		{			
			prevStates = newStates.exchange(stateSnapshot, std::memory_order_relaxed); //may change to release
		}));			
	if (prevStates)
	{			
		//delete[] prevStates->keys;
		//delete[] prevStates->values;
		delete prevStates;		
		prevStates = nullptr;		
	}
}

MFu64* ManifestRuntimeDatabase::PullStates()
{
	if (!newStates.load(std::memory_order_relaxed))
		return committedSimulation.xformTable;

	MFu64* prevStates{ committedSimulation.xformTable};
	//atomically updates simulation and removes flag
	stateLock.Read(std::function([&]()
		{	
			committedSimulation.xformTable =  			
			newStates.exchange(nullptr, std::memory_order_relaxed);//may change to release
		}));	
	//relase old memory 
	//delete[] prevStates->keys;
	//delete[] prevStates->values;
	delete prevStates;	
	prevStates = nullptr;

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

void ManifestRuntimeDatabase::INITIALIZE_FIRST_STORES__BYPASS_PULL_BRANCH()
{	
	//store dummy state data
	committedSimulation.xformTable = new MFu64;	
	//committedSimulation.xformTable = new XformTable;
	//committedSimulation.xformTable->keys = new UniqueKey;
	//committedSimulation.xformTable->values = new Xform;
}

MFu64* Manifest_Persistence::Simulate(const Simulation& simulation, const MFsize nBodies)
{
	for (auto object{ 0 }; object < nBodies; ++object)			++simulation.bodies.worldSpaces[object].field[13];
	//XformTable* result{ new XformTable };
	//result->tableEntries = result->tableSize = nBodies;
	//result->keys = new UniqueKey[nBodies];
	//result->values = new Xform[nBodies];
	//memcpy(result->values, simulation.bodies.worldSpaces, sizeof(Xform) * nBodies);
	//return result;	
	return new MFu64{simulation.simulationFrame};
}

FileSystemEventSpace FILESYSTEMEVENTSPACE;

void Manifest_Persistence::SimThread(ManifestRuntimeDatabase& runtimeDatabase)
{	
	DLOG(35, "SimThread ID: " << std::this_thread::get_id());
	/*auto nPhysicsObjects = runtimeDatabase.PullGeometryNodes()->geometryNodeTable.tableEntries;
	//nPhysicsObjects = 100000;
	Simulation simulation;	
	simulation.bodies.bodyID = new UniqueKey[nPhysicsObjects];
	simulation.bodies.worldSpaces = new Xform[nPhysicsObjects];
	memset(simulation.bodies.worldSpaces, 0, sizeof(Xform) * nPhysicsObjects);	
	//send initial push before allowing sim - removes need to check for prev state pointer as one will be gauranteed	
	runtimeDatabase.PushStates(Simulate(simulation, nPhysicsObjects));
	runtimeDatabase.init.test_and_set();
	runtimeDatabase.init.notify_one();
	//sleep and predicition
	auto simInterval = std::chrono::duration<double>{ 1/50.0 };
	auto begin = std::chrono::high_resolution_clock::now();	
	for (;;)
	{			
		auto prediction = begin + simulation.simulationFrame * simInterval;		
		//copy results to snapshot
		auto stateSnapshot = Simulate(simulation, nPhysicsObjects);
		//sync end simulation results - handles memory cleanup	
		runtimeDatabase.PushStates(stateSnapshot);
		//continue sim and push updates
		//update sim frame - concludes simulation for frame
		simulation.simulationFrame++;
		//sleep if permissible
		if (prediction > std::chrono::high_resolution_clock::now())
			std::this_thread::sleep_until(prediction);
	}*/
	constexpr FileSystemObservationToken simFSObservationToken{ UnderlyingType(FileSystem::MessageTypes::MBD_GEOMETRYNODE) };
	FileSystemObserver simFSObserver{ simFSObservationToken,FILESYSTEMEVENTSPACE.observerRegister };
}

void Manifest_Persistence::RenderThread(ManifestRuntimeDatabase& runtimeDatabase)
{	
	DLOG(35, "RenderThread ID : " << std::this_thread::get_id());
	/*auto set = false;
	if (!(set = runtimeDatabase.init.test()))
		runtimeDatabase.init.wait(set, std::memory_order_relaxed);
	MFu64 renderFrame = 0;	
	//represents the handle to the graphic resource which works with the world space data
	auto nObjects = runtimeDatabase.PullGeometryNodes()->geometryNodeTable.tableEntries;
	//nObjects = 100000;
	Xform* instancedVBOHandle = new Xform[nObjects];
	//sleep and predicition
	auto frameInterval = std::chrono::duration<double>{ 1 / 144.0 };
	auto begin = std::chrono::high_resolution_clock::now();	
	MFu64* stateSnapshot{nullptr};		
	for (;;)
	{
		auto prediction = begin + renderFrame * frameInterval;	
		auto currentStates = runtimeDatabase.PullStates();
		//get current simulation data - update vbo if new
		if (stateSnapshot != currentStates)
		{	
			stateSnapshot = currentStates;
			//memcpy(instancedVBOHandle, stateSnapshot->values, sizeof(Xform) * nObjects);
			RLOG(35, "Pulled simulation: " << *stateSnapshot);
		}
		//DLOG(35, "Render Frame: " << renderFrame << " simulation data: " << instancedVBOHandle[0].field[13]);		
		//update render frame
		renderFrame++;
		//sleep if permissible
		if (prediction > std::chrono::high_resolution_clock::now())
			std::this_thread::sleep_until(prediction);
	}*/	
	constexpr FileSystemObservationToken renderFSObservationToken{ UnderlyingType(FileSystemMessageType::MBD_GEOMETRYOBJECT | FileSystemMessageType::MBD_MATERIAL) };
	FileSystemObserver renderFSObserver{ renderFSObservationToken,FILESYSTEMEVENTSPACE.observerRegister };
}

void Manifest_Persistence::MessageThread()
{
	/*
	Binary_GeometryNode bNode_import;
	bNode_import.header.geometryID = 2;
	bNode_import.header.materialID = 2;
	bNode_import.header.nodeID = 2;
	bNode_import.header.payloadSize = 0;
	bNode_import.payload = nullptr;
	Binary_GeometryObject bObject_import;
	bObject_import.header.geometryID = 2;
	bObject_import.header.meshID = 2;
	bObject_import.header.morphID = KEY_NOT_PRESENT;
	bObject_import.header.payloadSize = 0;
	bObject_import.payload = nullptr;
	Binary_Material bMaterial_import;
	bMaterial_import.header.diffuseID = 2;
	bMaterial_import.header.materialID = 2;
	bMaterial_import.header.noramlID = KEY_NOT_PRESENT;
	bMaterial_import.header.diffuseID = KEY_NOT_PRESENT;
	bMaterial_import.header.payloadSize = sizeof(float) * 3;
	bMaterial_import.payload = new float[3];
	float* ptr = reinterpret_cast<float*>(bMaterial_import.payload);
	ptr[0] = 0;//r 
	ptr[1] = 1;//g 
	ptr[2] = 0;//b
	constexpr auto message1 = FileSystemMessageType::MDB_GEOMETRYNODE;
	constexpr auto message2 = FileSystemMessageType::MDB_GEOMETRYOBJECT;
	constexpr auto message3 = FileSystemMessageType::MDB_MATERIAL;

	xoshiro256ss_state rand;	
	auto sleepUntil = std::chrono::high_resolution_clock::now() + std::chrono::duration<double, std::milli > (1750);
	std::this_thread::sleep_until(sleepUntil);
	while (1)			
	{		
		FileSystemEvent fsEvent;
		switch (rand.Crunch() % 3)		
		{
			case 0:
				fsEvent.eventToken = UnderlyingType(message1 | message2 | message3);
				//event action 1			
				fsEvent.messages.emplace_back(Message{ bNode_import,UnderlyingType(message1) });
				//event action 2
				fsEvent.messages.emplace_back(Message{ bObject_import,UnderlyingType(message2) });
				//event action 3
				fsEvent.messages.emplace_back(Message{ bMaterial_import,UnderlyingType(message3) });
				//event action 1			
				fsEvent.messages.emplace_back(Message{ bNode_import,UnderlyingType(message1) });
				//event action 2
				fsEvent.messages.emplace_back(Message{ bObject_import,UnderlyingType(message2) });
				//event action 3
				fsEvent.messages.emplace_back(Message{ bMaterial_import,UnderlyingType(message3) });
				break;
			case 1:
				fsEvent.eventToken = UnderlyingType(message1);
				//event action 1			
				fsEvent.messages.emplace_back(Message{ bNode_import,UnderlyingType(message1) });
				fsEvent.messages.emplace_back(Message{ bNode_import,UnderlyingType(message1) });
				fsEvent.messages.emplace_back(Message{ bNode_import,UnderlyingType(message1) });
				fsEvent.messages.emplace_back(Message{ bNode_import,UnderlyingType(message1) });
				fsEvent.messages.emplace_back(Message{ bNode_import,UnderlyingType(message1) });
				break;
			case 2:				
				fsEvent.eventToken = UnderlyingType(message1 | message2 | message3);
				//event action 1			
				fsEvent.messages.emplace_back(Message{ bNode_import,UnderlyingType(message1) });
				//event action 2
				fsEvent.messages.emplace_back(Message{ bObject_import,UnderlyingType(message2) });
				//event action 3
				fsEvent.messages.emplace_back(Message{ bMaterial_import,UnderlyingType(message3) });
				break;
		}
		FILESYSTEMEVENTSPACE.NotifyRegisteredObservers(std::move(fsEvent));
	}
	*/
}

template<>
Materials* ManifestRuntimeDatabase::GetTable()
{
	return &materials;
}