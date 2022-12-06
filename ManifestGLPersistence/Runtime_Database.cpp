#include "Runtime_Database.h"

using namespace Manifest_Persistence;

ManifestRuntimeDatabase::ManifestRuntimeDatabase(const ManifestBinaryDatabase& binaryDatabase, const WorldSpaces& worldSpaces)
{
	xoshiro256ss_state ss;
	//store number of nodes and create	
	geometryNodes.instancedNodeIDs.tableSize = binaryDatabase.binaryGeometryNodeTable.header.totalEntries;
	geometryNodes.instancedNodeIDs.keys = new UniqueKey[geometryNodes.instancedNodeIDs.tableSize];
	geometryNodes.instancedNodeIDs.values = new PrimaryKey[geometryNodes.instancedNodeIDs.tableSize];
	//create storage for node geoemtry references
	geometryNodes.nodeGeometries = new PrimaryKey[geometryNodes.instancedNodeIDs.tableSize];
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
		geometryNodes.instancedNodeIDs.keys[geometryNodes.instancedNodeIDs.tableEntries] = node.header.nodeID;
		geometryNodes.instancedNodeIDs.values[geometryNodes.instancedNodeIDs.tableEntries] = ss.Crunch();		
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
		geometryNodes.nodeGeometries[geometryNodes.instancedNodeIDs.tableEntries++] = *instance;	
		//search for nodes materials
		auto mtlBegin = materials.materials.begin<PrimaryKey>();
		auto mtlEnd = materials.materials.end<PrimaryKey>();
		instance = std::find(mtlBegin, mtlEnd, node.header.materialID);
		if (instance == mtlEnd)
		{
			//get node material		
			const auto& material = binaryDatabase.binaryMaterialTable[node.header.materialID];
			instance = &(materials.materials.keys[materials.materials.tableEntries] = material.header.materialID);
			materials.materials.values[materials.materials.tableEntries].materialIDs[0] = material.header.diffuseID == KEY_NOT_PRESENT ? KEY_NOT_PRESENT : creationIndexCounter++;
			materials.materials.values[materials.materials.tableEntries].materialIDs[1] = material.header.noramlID == KEY_NOT_PRESENT ? KEY_NOT_PRESENT : creationIndexCounter++;
			materials.materials.values[materials.materials.tableEntries].materialIDs[2] = material.header.parallaxID == KEY_NOT_PRESENT ? KEY_NOT_PRESENT : creationIndexCounter++;
		}
		//refer node to material
		geometryNodes.nodeGeometries[geometryNodes.instancedNodeIDs.tableEntries++] = *instance;
	}
}