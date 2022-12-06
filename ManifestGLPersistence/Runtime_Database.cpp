#include "Runtime_Database.h"

using namespace Manifest_Persistence;

ManifestRuntimeDatabase::ManifestRuntimeDatabase(const ManifestBinaryDatabase& binaryDatabase, const WorldSpaces& worldSpaces)
{
	xoshiro256ss_state ss;
	//store number of nodes and create	
	geometryNodes.instancedNodeIDs.tableSize = binaryDatabase.binaryGeometryNodeTable.header.totalEntries;
	geometryNodes.instancedNodeIDs.keys = new UniqueKey[geometryNodes.instancedNodeIDs.tableSize];
	geometryNodes.instancedNodeIDs.values = new PrimaryKey[geometryNodes.instancedNodeIDs.tableSize];
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
		geometryNodes.instancedNodeIDs.tableEntries++;
		//search for nodes geometry
		auto instance = std::find(geometryObjects.geometryObjects.begin<PrimaryKey>(), geometryObjects.geometryObjects.end<PrimaryKey>(), node.header.geometryID);
		if (instance == geometryObjects.geometryObjects.end<PrimaryKey>())
		{
			//get node geometry
			const auto& geometry = binaryDatabase.binaryGeometryObjectTable[node.header.geometryID];
			//get node mesh
			const auto& mesh = binaryDatabase.binaryMeshTable[geometry.header.meshID];
			//add mesh to geometry runtime	
			geometryObjects.geometryObjects.keys[geometryObjects.geometryObjects.tableEntries] = geometry.header.geometryID;
			geometryObjects.geometryObjects.values[geometryObjects.geometryObjects.tableEntries] = creationIndexCounter++;//glCreateVertexArray
		}
		//create node mesh
		
		const auto& material = binaryDatabase.binaryMaterialTable[node.header.materialID];		
	}
}