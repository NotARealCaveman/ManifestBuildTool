#pragma once

#include <ManifestGLParser/OpenGEX_Parser.h>
#include <ManifestGLPersistence/BuildTool.h>
#include <ManifestGLUtility/Console_Color.h>
#include <ManifestGLPersistence/Manifest_DatabaseBuilder.h>
#include <ManifestGLPersistence/Runtime_Database.h>

#include <EXPERIMENTAL/Manifest_Allocator.h>
#include <EXPERIMENTAL/EXPERIMENTAL_RUNTIME_DATA_STRUCTURES.h>
#include <EXPERIMENTAL/EXPERIMENTAL_MESSAGING_STRUCTURES.h>

#include <thread>
#include <chrono>

using namespace Manifest_Parser;
using namespace Manifest_Persistence;
using namespace Manifest_Memory;
using namespace Manifest_Experimental;

const std::string TEST_PATH{ "C:\\Users\\Droll\\Desktop\\Game\\testing\\" };
const std::string TEST_GEX{ "Test1.gex" };
const std::string TEST_MDB{ "Test1.mdb" };

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
		DLOG(35, "begin<values>: " << ints.begin<>() << " end: " << ints.end<>());
		std::for_each(ints.begin<false>(), ints.end<false>(), [](const auto& i) {DLOG(32, i << " " << &i); });
		DLOG(35, "begin<keys>: " << ints.begin<false>() << " end: " << ints.end<false>());
	};
	DISABLE
		EqualTypeTest();

	const auto nNodes{ 5 };
	const auto nMeshes{ 3 };
	const auto nMaterials{ 3 };
	const auto nTextures{ 3 * nMaterials };

	//xforms	
	//WorldSpaces worldSpaces;
	//worldSpaces.xforms.tableSize = nNodes;
	//worldSpaces.xforms.tableEntries = 0;
	//worldSpaces.xforms.keys = new UniqueKey[nNodes];
	//worldSpaces.xforms.values = new Xform[nNodes];

	std::ifstream bImport{ TEST_PATH + TEST_MDB, std::ios::in | std::ios::binary };	
	ManifestRuntimeDatabase runtimeDatabase{ ImportBinaryDatabase(bImport) };	

	auto geometryObjects = runtimeDatabase.PullGeometryObjects();
	auto geometryNodes = runtimeDatabase.PullGeometryNodes();
	auto materials = runtimeDatabase.PullMaterials();

	for (auto nodeEntry = 0; nodeEntry < geometryNodes->geometryNodeTable.tableSize; ++nodeEntry)
	{			
		//get geometryObject id from the key address offset
		
		auto goBegin = geometryObjects->geometryObjectTable.begin<PrimaryKey>();
		auto goEnd = geometryObjects->geometryObjectTable.end<PrimaryKey>();
		
		auto goID = std::find(goBegin, goEnd, geometryNodes->nodeGeometries[nodeEntry]);
		auto index = goID - goBegin;
		auto VAO = geometryObjects->geometryObjectTable.values[index];
		auto mtlBegin = materials->materialTable.begin<PrimaryKey>();
		auto mtlEnd = materials->materialTable.end<PrimaryKey>();
		auto mtlID = std::find(mtlBegin, mtlEnd, geometryNodes->nodeMaterials[nodeEntry]);
		index = mtlID - mtlBegin;
		auto MTL = materials->materialTable.values[index];
		DLOG(31, "Geometry Node with id: " << geometryNodes->geometryNodeTable.values[nodeEntry] << " has VAO: " << VAO << " and MTL diffuse: " << MTL[0] <<" normal: " << MTL[1]<< " parallax: " << MTL[2]);
	}
} 

void PrintInfo(const DDL_Structure& structure)
{
	DLOG(33, "Structure " << structure.name << " with type: " << structure.identifier << " contains: " << structure.subSutructres.size() << " substructures");
	for (const auto& substructure : structure.subSutructres)
		PrintInfo(substructure);
};
	
void ImportAndTest()
{

	std::ifstream bImport{ TEST_PATH + TEST_MDB, std::ios::in | std::ios::binary };
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
		std::cout << std::endl;
		DLOG(31, "Material: " << importMaterial.header.materialID << " MTL(Diffuse): " << importMaterial.header.diffuseID << " MTL(Normal) : " << importMaterial.header.noramlID << " MTL(Parllax) : " << importMaterial.header.parallaxID);
		DLOG(35, "Texture info - w: " << importTexture.header.width << " h: " << importTexture.header.height << " nChannels: " << +importTexture.header.nChannels << " size: " << importTexture.header.payloadSize);		
		std::cout << "Texture Data: ";	
		auto nTextureElements{ importTexture.header.payloadSize / sizeof(MFfloat)};
		for (auto data{ 0 }; data < nTextureElements; ++data)
			std::cout << importTexture.payload[data] << ", ";
		std::cout << std::endl;		
	}
}

void BuildAndExport()
{
	//for printing purposes
	auto file = LoadFileContents(TEST_PATH + TEST_GEX);
	DLOG(31, file);
	auto filtered = FilterFile(file);
	DLOG(32, filtered);

	//begin actual parse
	//ddl start up
	Initialize_GEXTypes();
	Initialize_GEXGenerators();	
	DDL_File fileObject;
	//performs load and filter and begins parse
	ParseDDLFile(TEST_PATH + TEST_GEX, fileObject);
	//prints primary and substructure information per top level sturcture
	for (const auto& primary : fileObject.primaryStructures)
	{
		PrintInfo(primary);
		DLOG(31, "");
	}

	auto gn = fileObject.primaryStructures[3];
	const GEX_GeometryNode& node = *reinterpret_cast<GEX_GeometryNode*>(gn.typeHeap);
	const GEX_Transform& transform = node.transforms[0];
	const DDL_Float& data = transform.field;
	const DDL_Buffer& buffer = data.data;
	for (auto col = 0; col < 4; ++col)
	{
		for (auto row = 0; row < 4; ++row)
		{
			auto offset = row * 4 + col;
			auto field = reinterpret_cast<float*>(buffer.typeHeap);
			std::cout << field[offset]<<"  ";
		}
		std::cout << std::endl;
	}

	//NEXT TIME
	//build offline database
	ManifestDatabaseBuilder databaseBuilder;
	BuildOfflineDatabase(fileObject, databaseBuilder);
	//export conversion

	//export
	std::ofstream bExport{ TEST_PATH + TEST_MDB, std::ios::out | std::ios::binary };
	if (bExport.is_open())
	{
		ExportBinaryDatabase(databaseBuilder, bExport);
		bExport.close();
	}
}

void ThreadTest()
{
	std::ifstream bImport{ TEST_PATH + TEST_MDB, std::ios::in | std::ios::binary };
	ManifestRuntimeDatabase runtimeDatabase{ ImportBinaryDatabase(bImport) };
	std::thread rthread{ RenderThread,std::ref(runtimeDatabase) };
	SimThread(runtimeDatabase);//runs on main thread	
}

void MessageTest()
{
	ISub subscriber;
	Distributer distributer;
	Publisher publisher;

	publisher.distributer = &distributer;
	distributer.subscribers.emplace_back(&subscriber);

	publisher.PublishMessage(Message{NEW_MESSAGE});
	publisher.PublishMessage(Message{ NEW_MESSAGE });
	distributer.DistributeMessages();	
}


int main()
{	
	std::vector<int, LinearAllocator<int>> vecMFAlloc;	
	vecMFAlloc.emplace_back(1);

			
	WINDOWS_COLOR_CONSOLE;		

	MEMORYSTATUSEX status;
	status.dwLength = sizeof(status);
	GlobalMemoryStatusEx(&status);
	//std::cout<<status.ullTotalPhys << " " << status.ullAvailPhys;

	//db threading
	DISABLE
		MessageTest();
	DISABLE
		ThreadTest();
	//persistence tests
	DISABLE
		BuildAndExport();
	DISABLE
		ImportAndTest();
	//final
	DISABLE
		RuntimeTest();
	
	return 0;
}