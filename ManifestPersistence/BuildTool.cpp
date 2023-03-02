#include "BuildTool.h"

using namespace Manifest_Persistence;

void Manifest_Persistence::BuildResourceDatabase(const DDL_File& file, ManifestResourceDatabaseBuilder& database)
{		
	ScratchPadVector<DDL_Structure*>geometryObjects;
	ScratchPadVector<DDL_Structure*> materials;
	ScratchPadVector<DDL_Structure*> geometryNodes;
	//get all top level build structures
	for (const auto& structure : file.primaryStructures)
	{
		auto identifier = structure->identifier;
		auto mapEntry = DDL_BufferTypes::DDL_BufferTypeMap.find(identifier.c_str());
		if (mapEntry != DDL_BufferTypes::DDL_BufferTypeMap.end())
		{
			auto bufferType = mapEntry->second;
			switch (bufferType)
			{
				case GEX_BufferTypes::GEX_GeometryObject:		
					geometryObjects.emplace_back(structure);
					break;
				case GEX_BufferTypes::GEX_Material:
					materials.emplace_back(structure);
					break;
				case GEX_BufferTypes::GEX_GeometryNode:
					geometryNodes.emplace_back(structure);				
					break;
				default:break;//prunes pure transmission structures
			}
		}
	}		
	//build geometry object and material tables first then nodes as they require references - split like this as if the database build becomes very large then these can be split into parallel parts and built together and then when complete the single thread can take over the references	
	for (const auto& geometryObject : geometryObjects)
		TableEntry(*geometryObject, database.geometryObjectBuildTable, database.meshBuildTable, database.vertexBuildTables, database.indexBuildTable);	
	for (const auto& material : materials)
		TableEntry(*material, database.materialBuildTable, database.textureBuildTable);	
	for (const auto& geometryNode : geometryNodes)
		TableEntry(*geometryNode, database.geometryObjectBuildTable, database.materialBuildTable, database.geometryNodeBuildTable, database.objectRefBuildTable, database.materialRefBuildTable);
}

void Manifest_Persistence::BuildTerrainDatabase(const DDL_File& file, ManifestTerrainDatabaseBuilder& database)
{
	ScratchPadVector<DDL_Structure*> terrainChunks;
	ScratchPadVector<DDL_Structure*> chunkMetrics;
	for (const auto& structure : file.primaryStructures)
	{
		auto identifier = structure->identifier;
		auto mapEntry = DDL_BufferTypes::DDL_BufferTypeMap.find(identifier.c_str());
		if (mapEntry != DDL_BufferTypes::DDL_BufferTypeMap.end())
		{
			auto bufferType = mapEntry->second;
			switch (bufferType)
			{
			case DDL_ExtendedTypes::MDD_TERRAIN:
				terrainChunks.emplace_back(structure);
				break;
			case GEX_BufferTypes::GEX_Metric:
				chunkMetrics.emplace_back(structure);
				break;
			}
		}
	}
	for (const auto& terrainChunk : terrainChunks)
		TableEntry(*terrainChunk, database.terrainBuildTable);
}