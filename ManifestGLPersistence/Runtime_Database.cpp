#include "Runtime_Database.h"

using namespace Manifest_Persistence;

ManifestRuntimeDatabase::ManifestRuntimeDatabase(const ManifestBinaryDatabase& binaryDatabase, const WorldSpaces& worldSpaces, const GraphicResources& graphicResources)
{
	const auto& bGNnodeTable = binaryDatabase.binaryGeometryNodeTable;
	auto nNodes = bGNnodeTable.header.totalEntries;
	geometryNodes.instancedNodeIDs.tableSize = nNodes;
	geometryNodes.instancedNodeIDs.keys = new UniqueKey[nNodes];
	geometryNodes.instancedNodeIDs.values = new PrimaryKey[nNodes];
	geometryNodes.vaoRefs.tableSize = nNodes;
	geometryNodes.vaoRefs.keys = new PrimaryKey[nNodes];
	geometryNodes.vaoRefs.values = new GraphicID*[nNodes];

	auto& i = geometryNodes.instancedNodeIDs.tableEntries;
	for (;i< nNodes; ++i)
	{
		const auto& node = binaryDatabase.binaryGeometryNodeTable[i];
		geometryNodes.vaoRefs.keys[i] = node.header.geometryID;
		int index = 0;
		while (graphicResources.VAOs.keys[index] != node.header.geometryID)
		{
			DLOG(31, graphicResources.VAOs.keys[index]);
			++index;
		}		
		geometryNodes.vaoRefs.values[i] = &graphicResources.VAOs.values[index];		
		++geometryNodes.vaoRefs.tableEntries;
	}
}