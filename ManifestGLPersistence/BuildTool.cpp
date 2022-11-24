#include "BuildTool.h"

using namespace Manifest_Persistence;

void Manifest_Persistence::BuildOfflineDatabase(const DDL_File& file, ManifestDatabaseBuild& database)
{		
	std::vector<DDL_Structure> geometryObjects;
	std::vector<DDL_Structure> materials;
	std::vector<DDL_Structure> geometryNodes;
	for (const auto& structure : file.primaryStructures)
	{
		auto identifier = structure.identifier;
		auto mapEntry = DDL_BufferTypes::DDL_BufferTypeMap.find(identifier);
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
				default:break;//default_break macro unused, prunes pure transmission structures
			}
		}
	}		
	//build geometry object and material tables first then nodes as they require references - split like this as if the database build becomes very large then these can be split into parallel parts and built together and then when complete the single thread can take over the references
	for (const auto& geometryObject : geometryObjects)
		TableEntry(geometryObject, database.geometryObjectTable, database.meshTable, database.vertexTables, database.indexTable);
	for (const auto& material : materials)
		TableEntry(material, database.materialTable, database.colorTable);
	for (const auto& geometryNode : geometryNodes)
		TableEntry(geometryNode, database.geometryObjectTable, database.materialTable, database.geometryNodeTable, database.objectRefTable, database.materialRefTable);
}

