#pragma once

#include <ManifestGLParser/OpenGEX_Parser.h>
#include <ManifestGLPersistence/BuildTool.h>
#include <ManifestGLUtility/Console_Color.h>
#include <ManifestGLPersistence/MDB/Binary/Binary_Types.h>

#include <EXPERIMENTAL/Manifest_Allocator.h>

using namespace Manifest_Parser;
using namespace Manifest_Persistence;
using namespace Manifest_Memory;

//* - IN PROGRESS ** - FINISHED ? - DESIGN QUESTION
//TODO: MDB_COLOR/TEXTURE*
//TODO: BINARY COLOR/TEXTURE/MATERIAL*
//TODO: RUNTIME DATABASE FILE HEADER
//TODO: EX/IMPORT ORDER*
//TODO: MEMORY ALLOCATOR*
//TODO: TABLE INDEXING AND POINTER OFFSETS
//TODO: RUNTIME DATABASE VS BINARY DATABASE ?

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

		//export
		std::ofstream bExport{ "C:\\Users\\Droll\\Desktop\\Game\\testoimng\\TEST.mdb", std::ios::out | std::ios::binary };
		if (bExport.is_open())
		{
			ExportRuntimeDatabase(database, bExport);
			bExport.close();
		}
		//test import 
		std::ifstream bImport{ "C:\\Users\\Droll\\Desktop\\Game\\testoimng\\TEST.mdb", std::ios::in | std::ios::binary };
		ManifestRuntimeDatabase runtimeDatabase = ImportRuntimeDatabase(bImport);
		//DISABLE
		{
			for (auto i = 0; i < runtimeDatabase.binaryGeometryNodeTable.header.totalEntries; ++i)
			{
				const auto& importObject = runtimeDatabase.binaryGeometryNodeTable[i];
				const auto& importGeometry = runtimeDatabase.binaryGeometryObjectTable[importObject.header.geometryID];
				const auto& importMesh = runtimeDatabase.binaryMeshTable[importGeometry.header.meshID];
				const auto& importTexture = runtimeDatabase.binaryTextureTable[importObject.header.materialID];
				DLOG(37, "Reading from file:" << i);
				DLOG(33, "Mesh Stride:" << importMesh.header.vboStride << " AttributeCode: " << +importMesh.header.activeArrayAttributes << " Vertex Buffer Size: " << importMesh.header.payloadSize << " bytes Vertex Buffer Elements: " << importMesh.header.payloadSize / sizeof(float) << " texture channels: " << +importTexture.header.nChannels);

				auto nElements = (importMesh.header.eboOffset) / sizeof(float);
				auto nSubArrayElements = nElements / importMesh.header.vboStride;
				auto count{ 0 };
				for (auto subArray = 0; subArray < nSubArrayElements; ++subArray)
				{
					std::cout << "{";
					for (auto arrayIndex = 0; arrayIndex < importMesh.header.vboStride; ++arrayIndex)
						std::cout << importMesh.payload[subArray * importMesh.header.vboStride + arrayIndex] << " ,";
					std::cout << "}" << std::endl;
				}
				auto nIndices = (importMesh.header.payloadSize - importMesh.header.eboOffset) / sizeof(uint32_t);
				char* beginIndices = reinterpret_cast<char*>(importMesh.payload) + importMesh.header.eboOffset;
				std::cout << "{";
				for (auto index = 0; index < nIndices; ++index)
					std::cout << reinterpret_cast<uint32_t*>(beginIndices)[index] << ",";
				std::cout << "}" << std::endl;
			}
		}
	};
	Manifest_Allocator allocator;
	allocator.CreateHeap(64);
	double long * dbllng = new(allocator.Alloc(sizeof(double long)))double long{ 65 };
	DLOG(32, "dbllng: " << dbllng);
	int* intptr2 = new(allocator.Alloc(sizeof(int)))int{ 32 };
	DLOG(32, "intptr2: " << intptr2);
	allocator.Dealloc(dbllng);
	float* floatptr = new(allocator.Alloc(sizeof(float)))float{ 4.20 };
	DLOG(32, "floatptr: " << floatptr);
	float* floatptr2 = new(allocator.Alloc(sizeof(float)))float{ 6.9 };
	DLOG(32, "floatptr2: " << floatptr2);
	allocator.Dealloc(floatptr);
	double long* dbllng2 = new(allocator.Alloc(sizeof(double long)))double long{ 1488 };
	DLOG(32, "dbllng2: " << dbllng2);
	return 0;
}