#include "Runtime_Database.h"

using namespace Manifest_Persistence;

ManifestRuntimeDatabase::ManifestRuntimeDatabase(const ManifestBinaryDatabase& binaryDatabase, const WorldSpaces& worldSpaces, const GraphicResources& graphicResources)
{
	const auto& bGNnodeTable = binaryDatabase.binaryGeometryNodeTable;
	auto nNodes = bGNnodeTable.header.totalEntries;
	
}