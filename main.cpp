#pragma once

#include <ManifestGLParser/OpenGEX_Parser.h>
#include <ManifestGLPersistence/BuildTool.h>
#include <ManifestGLUtility/Console_Color.h>
#include <ManifestGLPersistence/MDB/Binary/Binary_Mesh.h>

using namespace Manifest_Parser;
using namespace Manifset_Persistence;

int main()
{
	WINDOWS_COLOR_CONSOLE
	//ddl start up
	Initialize_GEXTypes();
	Initialize_GEXGenerators();
	//parse
	DDL_File fileObject;
	
	ParseDDLFile("", fileObject);
	
	//build database
	ManifestDatabase database;
	BuildDatabase(fileObject, database);
	//export conversion
	const BinaryMeshTable exportTable{ BuildBinaryTable(database.meshTable,database.vertexTables,database.indexTable) };
	//export
	std::ofstream bExport{ "C:\\Users\\Droll\\Desktop\\Game\\testoimng\\TEST.mdb", std::ios::out | std::ios::binary };
	if (bExport.is_open())
	{		
		ExportBinaryTable(exportTable, bExport);		
		bExport.close();
	}
	//import 
	std::ifstream bImport{ "C:\\Users\\Droll\\Desktop\\Game\\testoimng\\TEST.mdb", std::ios::in | std::ios::binary };
	BinaryMeshTable importTable;
	if (bImport.is_open())
	{		
		importTable = ImportBinaryTable<Binary_Mesh>(bImport);		
		bImport.close();
	}
	//test
	{
		auto importMesh = importTable[0];
		DLOG(37, "Reading from file!!!");
		DLOG(33, "Mesh Stride:" << importMesh->header.vboStride << " AttributeCode: " << +importMesh->header.activeArrayAttributes << " Vertex Buffer Size: " << importMesh->header.payloadSize << " bytes Vertex Buffer Elements: " << importMesh->header.payloadSize / sizeof(float));

		auto nElements = importMesh->header.payloadSize / sizeof(float);
		auto nSubArrayElements = nElements / importMesh->header.vboStride;
		for (auto subArray = 0; subArray < nSubArrayElements; ++subArray)
		{
			std::cout << "{";
			for (auto arrayIndex = 0; arrayIndex < importMesh->header.vboStride; ++arrayIndex)
			{
				std::cout << importMesh->payload[subArray * importMesh->header.vboStride + arrayIndex] << " ,";
			}
			std::cout << "}" << std::endl;
		}
	}
	
	return 0;
}