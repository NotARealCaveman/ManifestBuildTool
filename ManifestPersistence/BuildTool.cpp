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

void Manifest_Persistence::BuildWorldDatabase(const DDL_File& file, ManifestWorldDatabaseBuilder& database)
{
	ScratchPadVector<DDL_Structure*> terrainChunks;
	ScratchPadVector<DDL_Structure*> voxelMaps;
	ScratchPadVector<DDL_Structure*> worldMetrics;	
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
				case DDL_ExtendedTypes::MDD_VOXELMAP:
					voxelMaps.emplace_back(structure);
					break;
				case GEX_BufferTypes::GEX_Metric:
					worldMetrics.emplace_back(structure);
					break;
			}
		}
	}	
	for (const auto& terrainChunk : terrainChunks)
		TableEntry(*terrainChunk, database.terrainBuildTable);	
	//in the future metrics will be used as world config - for now combine data into voxel map	
	MFu32 xChunks;
	MFu32 zChunks;
	MFu8 worldLOD;
	for (const auto& worldMetric : worldMetrics)
	{
		const auto& metric{ HeapData<GEX_Metric>(*worldMetric) };
		if (metric.key == "\"lod\"")
		{
			worldLOD = *reinterpret_cast<MFu8*>(std::get<DDL_Uint8>(metric.metric).data.typeHeap);
		}
		else if (metric.key == "\"xChunks\"")
		{
			worldLOD = *reinterpret_cast<MFu32*>(std::get<DDL_Uint32>(metric.metric).data.typeHeap);
		}
		else if (metric.key == "\"zChunks\"")
		{
			worldLOD = *reinterpret_cast<MFu32*>(std::get<DDL_Uint32>(metric.metric).data.typeHeap);
		}
	}
	for (const auto& voxelMap : voxelMaps)
		TableEntry(*voxelMap, database.voxelMapBuildTable,xChunks,zChunks,worldLOD);
}