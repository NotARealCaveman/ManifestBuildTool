#include "Manifest_Database.h"

using namespace Manifest_Persistence;


void Manifest_Persistence::ExportRuntimeDatabase(const ManifestDatabaseBuild& databaseBuild, std::ofstream& exportFile)
{
	//TODO: ADD TABLE HEADER FOR QUICK LOOK UP - SEE [] OVERLOAD IN TABLE FOR AN EXAMPLE
	//--		
	//convert mdbs into binary records
	ManifestRuntimeDatabase exportDatabase;
	exportDatabase.binaryMeshTable =  BinaryTableConversion<Binary_Mesh, MeshTable, VertexTables, IndexTable>(databaseBuild.meshTable, databaseBuild.vertexTables, databaseBuild.indexTable) ;
	exportDatabase.binaryGeometryObjectTable = BinaryTableConversion<Binary_GeometryObject, GeometryObjectTable>(databaseBuild.geometryObjectTable);
	exportDatabase.binaryGeometryNodeTable = BinaryTableConversion<Binary_GeometryNode, GeometryNodeTable, ObjectRefTable, MaterialRefTable>(databaseBuild.geometryNodeTable, databaseBuild.objectRefTable, databaseBuild.materialRefTable);
	//export binary tables to file
	//TODO: settle on table ordering as ideally the entire table will be loaded into memory and a file header will be parsed to build the various table entrie payloads
	ExportBinaryTable(exportDatabase.binaryMeshTable, exportFile);
	ExportBinaryTable(exportDatabase.binaryGeometryObjectTable, exportFile);
	ExportBinaryTable(exportDatabase.binaryGeometryNodeTable, exportFile);
}

ManifestRuntimeDatabase Manifest_Persistence::ImportRuntimeDatabase(std::ifstream& importFile)
{
	//TODO: once memory allocator is written all tables will be tightly packed in memory - this means that the import order must match the export order of the file as the database will be loaded with a single read - see header for additional database table creation manipulation
	ManifestRuntimeDatabase result;

	result.binaryMeshTable = ImportBinaryTable<Binary_Mesh>(importFile);	
	result.binaryGeometryObjectTable = ImportBinaryTable<Binary_GeometryObject>(importFile);
	result.binaryGeometryNodeTable = ImportBinaryTable<Binary_GeometryNode>(importFile);

	return result;
}

ManifestRuntimeDatabase::ManifestRuntimeDatabase(ManifestRuntimeDatabase&& other)
	:	binaryMeshTable{ std::move(other.binaryMeshTable) },
		binaryGeometryNodeTable{std::move(other.binaryGeometryNodeTable)},
		binaryGeometryObjectTable{std::move(other.binaryGeometryObjectTable)}
{}