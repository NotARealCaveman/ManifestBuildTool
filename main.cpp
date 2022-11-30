#pragma once

#include <ManifestGLParser/OpenGEX_Parser.h>
#include <ManifestGLPersistence/BuildTool.h>
#include <ManifestGLUtility/Console_Color.h>
#include <ManifestGLPersistence/Manifest_DatabaseBuilder.h>
#include <ManifestGLPersistence/Runtime_Database.h>

#include <EXPERIMENTAL/Manifest_Allocator.h>
#include <EXPERIMENTAL/EXPERIMENTAL_RUNTIME_DATA_STRUCTURES.h>

using namespace Manifest_Parser;
using namespace Manifest_Persistence;
using namespace Manifest_Memory;

//* - IN PROGRESS ** - FINISHED ? - DESIGN QUESTION
//TODO: MDB_COLOR/TEXTURE**
//TODO: BINARY COLOR/TEXTURE/MATERIAL**
//TODO: BINARY IMPORT DATABASE FILE HEADER
//TODO: EX/IMPORT ORDER*
//TODO: MEMORY ALLOCATOR*
//TODO: TABLE INDEXING AND POINTER OFFSETS
//TODO: RUNTIME DATABASE VS BINARY DATABASE *

void Test()
{
	//DISABLE
	{
		const auto nNodes{ 5 };
		const auto nMeshes{ 3 };
		const auto nMaterials{ 3 };
		const auto nTextures{ 3 * nMaterials };

		//xforms
		WorldSpaces worldSpaces;
		worldSpaces.xforms.tableSize = nNodes;
		worldSpaces.xforms.tableEntries = 0;
		worldSpaces.xforms.keys = new UniqueKey[nNodes];
		worldSpaces.xforms.values = new Xform[nNodes];

		GraphicResources graphicResources;
		//textures
		graphicResources.tIDs.tableSize = nMeshes;
		graphicResources.tIDs.tableEntries = 0;
		graphicResources.tIDs.keys = new UniqueKey[nMeshes];
		graphicResources.tIDs.values = new MFu32[nNodes];



		//meshes
		graphicResources.VAOs.tableSize = nMeshes;
		graphicResources.VAOs.tableEntries = 0;
		graphicResources.VAOs.keys = new UniqueKey[nMeshes];
		graphicResources.VAOs.values = new MFu32[nMeshes];


		std::ifstream bImport{ "C:\\Users\\Droll\\Desktop\\Game\\testoimng\\TEST2.mdb", std::ios::in | std::ios::binary };
		ManifestBinaryDatabase binaryDatabase = ImportBinaryDatabase(bImport);

		int idCounter{ 0 };
		//pair manifest ids to runtime ids
		for (int i = 0; i < nMeshes; ++i)
		{
			//check manifest mesh ID against current table entries
			auto& VAOTable = graphicResources.VAOs;
			auto begin = VAOTable.begin<PrimaryKey>();
			auto end = VAOTable.end<PrimaryKey>();
			auto gnMtid = binaryDatabase.binaryGeometryNodeTable[i].header.materialID;
			PrimaryKey key = binaryDatabase.binaryMeshTable[gnMtid].header.meshID;
			DLOG(32, "mtID: " << gnMtid << " key: " << key);
			//search for manifest key 
			auto entry = std::find(begin, end, key);
			//key not in table - add and pair with runtime generated id
			if (entry == end)
			{
				VAOTable.keys[VAOTable.tableEntries] = key;
				VAOTable.values[VAOTable.tableEntries] = idCounter++;
				VAOTable.tableEntries++;
			}
		}

		ManifestRuntimeDatabase database(binaryDatabase, worldSpaces, graphicResources);
	}
}

int main()
{
	WINDOWS_COLOR_CONSOLE;
	
	Test();

	DISABLE
	{
		//BUILDS AND EXPORTS DATABASE
		//DISABLE
		{
			//ddl start up
			Initialize_GEXTypes();
			Initialize_GEXGenerators();
			//parse
			DDL_File fileObject;

			ParseDDLFile("", fileObject);

			//build offline database
			ManifestDatabaseBuilder databaseBuilder;
			BuildOfflineDatabase(fileObject, databaseBuilder);
			//export conversion

			//export
			std::ofstream bExport{ "C:\\Users\\Droll\\Desktop\\Game\\testoimng\\TEST2.mdb", std::ios::out | std::ios::binary };
			if (bExport.is_open())
			{
				ExportBinaryDatabase(databaseBuilder, bExport);
				bExport.close();
			}
		}
		//IMPORTS DATABASE AND TEST
		std::ifstream bImport{ "C:\\Users\\Droll\\Desktop\\Game\\testoimng\\TEST2.mdb", std::ios::in | std::ios::binary };
		{
			ManifestBinaryDatabase binaryDatabase  = ImportBinaryDatabase(bImport);		
			for (auto i = 0; i < binaryDatabase.binaryGeometryNodeTable.header.totalEntries; ++i)
			{
				const auto importObject = binaryDatabase.binaryGeometryNodeTable[i];
				const auto importGeometry = binaryDatabase.binaryGeometryObjectTable[importObject.header.geometryID];
				const auto importMesh = binaryDatabase.binaryMeshTable[importGeometry.header.meshID];
				const auto importMaterial = binaryDatabase.binaryMaterialTable[importObject.header.materialID];
				const auto importTexture = binaryDatabase.binaryTextureTable[importMaterial.header.diffuseID];
				//DLOG(37, "Reading from file:" << i);
				//DLOG(33, "Mesh Stride:" << importMesh.header.vboStride << " AttributeCode: " << +importMesh.header.activeArrayAttributes << " Vertex Buffer Size: " << importMesh.header.payloadSize << " bytes Vertex Buffer Elements: " << importMesh.header.payloadSize / sizeof(float) << " texture channels: " << +importTexture.header.nChannels <<" texture size: " << +importTexture.header.payloadSize);

				auto nElements = (importMesh.header.eboOffset) / sizeof(float);
				auto nSubArrayElements = nElements / importMesh.header.vboStride;
				auto count{ 0 };
				DISABLE
				for (auto subArray = 0; subArray < nSubArrayElements; ++subArray)
				{
					std::cout << "{";
					for (auto arrayIndex = 0; arrayIndex < importMesh.header.vboStride; ++arrayIndex)
						std::cout << importMesh.payload[subArray * importMesh.header.vboStride + arrayIndex] << " ,";
					std::cout << "}" << std::endl;
				}
				auto nIndices = (importMesh.header.payloadSize - importMesh.header.eboOffset) / sizeof(uint32_t);
				char* beginIndices = reinterpret_cast<char*>(importMesh.payload) + importMesh.header.eboOffset;
				DISABLE
				for (auto index = 0; index < nIndices; ++index)
					std::cout << reinterpret_cast<uint32_t*>(beginIndices)[index] << ",";
				
			}
		}
	};	
	return 0;
}