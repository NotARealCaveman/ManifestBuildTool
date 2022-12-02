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

void RuntimeTest()
{
	const auto& EqualTypeTest = []()
	{
		Table<int, int> ints;
		ints.tableEntries = ints.tableSize = 3;
		ints.keys = new int[ints.tableSize]; ints.keys[0] = 3; ints.keys[1] = 4; ints.keys[2] = 5;
		ints.values = new int[ints.tableSize]; ints.values[0] = 0; ints.values[1] = 1; ints.values[2] = 2;

		DLOG(34, "values: " << ints.values << " keys: " << ints.keys);
		std::for_each(ints.begin<>(), ints.end<>(), [](const auto& i) {DLOG(31, i << " " << &i); });
		DLOG(35, "begin<values>: " << ints.begin<>() << " end: " << ints.begin<>());
		std::for_each(ints.begin<false>(), ints.end<false>(), [](const auto& i) {DLOG(32, i << " " << &i); });
		DLOG(35, "begin<keys>: " << ints.begin<false>() << " end: " << ints.begin<false>());
	};
	DISABLE	EqualTypeTest();

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
	graphicResources.tIDs.keys = new UniqueKey[nMaterials];
	graphicResources.tIDs.values = new MFu32[nMaterials];

	//meshes
	graphicResources.VAOs.tableSize = nMeshes;
	graphicResources.VAOs.tableEntries = 0;
	graphicResources.VAOs.keys = new UniqueKey[nMeshes];
	graphicResources.VAOs.values = new MFu32[nMeshes];

	std::ifstream bImport{ "C:\\Users\\Droll\\Desktop\\Game\\testoimng\\TEST2.mdb", std::ios::in | std::ios::binary };
	ManifestBinaryDatabase binaryDatabase = ImportBinaryDatabase(bImport);

	int idCounter{ 0 };
	//pair manifest ids to runtime ids
	for (int i = 0; i < nNodes; ++i)
	{
		//check manifest mesh ID against current table entries
		auto& VAOTable = graphicResources.VAOs;
		VAOTable.begin<PrimaryKey>();
		auto begin = VAOTable.begin<PrimaryKey>();
		auto end = VAOTable.end<PrimaryKey>();
		auto gnMtid = binaryDatabase.binaryGeometryNodeTable[i].header.geometryID;
		PrimaryKey key = binaryDatabase.binaryMeshTable[gnMtid].header.meshID;
		//search for mesh with manifest key 
		auto geometry = std::find(begin, end, key);
		//key not in table - add and pair with runtime generated id
		if (geometry == end)
		{
			VAOTable.keys[VAOTable.tableEntries] = key;
			VAOTable.values[VAOTable.tableEntries] = idCounter++;
			VAOTable.tableEntries++;
		}
		//repeat for materials
		auto& tIDTable = graphicResources.tIDs;
		begin = tIDTable.begin<PrimaryKey>();
		end = tIDTable.end<PrimaryKey>();
		gnMtid = binaryDatabase.binaryGeometryNodeTable[i].header.materialID;
		key = binaryDatabase.binaryMaterialTable[gnMtid].header.diffuseID;
		if (std::find(begin, end, key) == end && key != KEY_NOT_PRESENT)
		{
			tIDTable.keys[tIDTable.tableEntries] = key;
			tIDTable.values[tIDTable.tableEntries] = idCounter++;
			tIDTable.tableEntries++;
		}
		key = binaryDatabase.binaryMaterialTable[gnMtid].header.noramlID;
		if ((std::find(begin, end, key) == end) && (key != KEY_NOT_PRESENT))
		{
			DLOG(36, "(key != KEY_NOT_PRESENT): " << (key != KEY_NOT_PRESENT) << ": " << "(" << key << " != " << KEY_NOT_PRESENT << ")");
			tIDTable.keys[tIDTable.tableEntries] = key;
			tIDTable.values[tIDTable.tableEntries] = idCounter++;
			tIDTable.tableEntries++;
		}
		key = binaryDatabase.binaryMaterialTable[gnMtid].header.parallaxID;
		if (std::find(begin, end, key) == end && key != KEY_NOT_PRESENT)
		{
			tIDTable.keys[tIDTable.tableEntries] = key;
			tIDTable.values[tIDTable.tableEntries] = idCounter++;
			tIDTable.tableEntries++;
		}
	}

	std::for_each(graphicResources.VAOs.begin<PrimaryKey>(), graphicResources.VAOs.end<PrimaryKey>(), [](const auto& id) {DLOG(36, "Mesh Resource key: " << id); });
	std::for_each(graphicResources.VAOs.begin<GraphicID>(), graphicResources.VAOs.end<GraphicID>(), [](const auto& id) {DLOG(36, "Mesh Resource ID: " << id); });	
	//link database to runtime structures
	ManifestRuntimeDatabase database(binaryDatabase, worldSpaces, graphicResources);
	auto begin = database.geometryNodes.vaoRefs.begin<GraphicID*>();	
	auto end = database.geometryNodes.vaoRefs.end<GraphicID*>();
	DLOG(37, "range begin: " << begin << " end: " << end);
	std::for_each(begin, end, [](const GraphicID* vao) {DLOG(31, "runtime pointed vao: " << *vao << " &"<<vao); });
}


void BuildAndExport()
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
	
void ImportAndTest()
{

	std::ifstream bImport{ "C:\\Users\\Droll\\Desktop\\Game\\testoimng\\TEST2.mdb", std::ios::in | std::ios::binary };
	ManifestBinaryDatabase binaryDatabase = ImportBinaryDatabase(bImport);
	for (auto i = 0; i < binaryDatabase.binaryGeometryNodeTable.header.totalEntries; ++i)
	{
		const auto importObject = binaryDatabase.binaryGeometryNodeTable[i];
		const auto importGeometry = binaryDatabase.binaryGeometryObjectTable[importObject.header.geometryID];
		const auto importMesh = binaryDatabase.binaryMeshTable[importGeometry.header.meshID];
		const auto importMaterial = binaryDatabase.binaryMaterialTable[importObject.header.materialID];
		const auto importTexture = binaryDatabase.binaryTextureTable[importMaterial.header.diffuseID];
		DLOG(37, "Reading from file:" << i);
		DLOG(33, "Mesh Stride:" << importMesh.header.vboStride << " AttributeCode: " << +importMesh.header.activeArrayAttributes << " Vertex Buffer Size: " << importMesh.header.payloadSize << " bytes Vertex Buffer Elements: " << importMesh.header.payloadSize / sizeof(float) << " texture channels: " << +importTexture.header.nChannels << " texture size: " << +importTexture.header.payloadSize);

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
		for (auto index = 0; index < nIndices; ++index)
			std::cout << reinterpret_cast<uint32_t*>(beginIndices)[index] << ",";

	}
}

int main()
{
	WINDOWS_COLOR_CONSOLE;
	
	//DISABLE
		BuildAndExport();
	DISABLE
		ImportAndTest();
	//DISABLE
		RuntimeTest();
	
	return 0;
}