#pragma once

#include <ManifestGLParser/OpenGEX_Parser.h>
#include <ManifestGLPersistence/BuildTool.h>
#include <ManifestGLUtility/Console_Color.h>
#include <ManifestGLPersistence/MDB/Binary/Binary_Types.h>

using namespace Manifest_Parser;
using namespace Manifest_Persistence;

int main()
{	
	WINDOWS_COLOR_CONSOLE
	DISABLE
	{	
	//ddl start up
	Initialize_GEXTypes();
	Initialize_GEXGenerators();
	//parse
	DDL_File fileObject;

	ParseDDLFile("", fileObject);

	//build offline database
	ManifestDatabaseBuild database;
	BuildOfflineDatabase(fileObject, database);
	//export conversion
	const BinaryMeshTable exportTable{ BinaryTableConversion<Binary_Mesh,MeshTable,VertexTables,IndexTable>(database.meshTable,database.vertexTables,database.indexTable) };
	const BinaryGeometryObjectTable exportTable2{ BinaryTableConversion<Binary_GeometryObject,GeometryObjectTable>(database.geometryObjectTable) };
	const BinaryGeometryNodeTable exportTable3{ BinaryTableConversion<Binary_GeometryNode,GeometryNodeTable,ObjectRefTable,MaterialRefTable>(database.geometryNodeTable,database.objectRefTable,database.materialRefTable)};
	//export
	std::ofstream bExport{ "C:\\Users\\Droll\\Desktop\\Game\\testoimng\\TEST.mdb", std::ios::out | std::ios::binary };
	if (bExport.is_open())
	{
		ExportRuntimeDatabase(database, bExport);
		bExport.close();
	}
	}
	//test import 
	std::ifstream bImport{ "C:\\Users\\Droll\\Desktop\\Game\\testoimng\\TEST.mdb", std::ios::in | std::ios::binary };
	ManifestRuntimeDatabase runtimeDatabase = ImportRuntimeDatabase(bImport);
	DLOG(32, &runtimeDatabase << " returned");
	//DISABLE
	{
		for (auto i = 0; i < runtimeDatabase.binaryGeometryNodeTable.header.totalEntries; ++i)
		{
			auto importObject = runtimeDatabase.binaryGeometryNodeTable[i];
			auto importGeometry = runtimeDatabase.binaryGeometryObjectTable[importObject->header.geometryID];
			auto importMesh = runtimeDatabase.binaryMeshTable[importGeometry->header.meshID];
			DLOG(37, "Reading from file:" << i );
			DLOG(33, "Mesh Stride:" << importMesh->header.vboStride << " AttributeCode: " << +importMesh->header.activeArrayAttributes << " Vertex Buffer Size: " << importMesh->header.payloadSize << " bytes Vertex Buffer Elements: " << importMesh->header.payloadSize / sizeof(float));

			auto nElements = (importMesh->header.eboOffset) / sizeof(float);
			auto nSubArrayElements = nElements / importMesh->header.vboStride;
			auto count{ 0 };
			for (auto subArray = 0; subArray < nSubArrayElements; ++subArray)
			{
				std::cout << "{";
				for (auto arrayIndex = 0; arrayIndex < importMesh->header.vboStride; ++arrayIndex)
					std::cout << importMesh->payload[subArray * importMesh->header.vboStride + arrayIndex] << " ,";
				std::cout << "}" << std::endl;
			}
			auto nIndices = (importMesh->header.payloadSize - importMesh->header.eboOffset) / sizeof(uint32_t);
			char* beginIndices = reinterpret_cast<char*>(importMesh->payload) + importMesh->header.eboOffset;
			std::cout << "{";
			for (auto index = 0; index < nIndices; ++index)
				std::cout << reinterpret_cast<uint32_t*>(beginIndices)[index] << ",";
			std::cout << "}" << std::endl;
		}
	}
	
	return 0;
}