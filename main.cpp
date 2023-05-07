#pragma once

#include <ManifestParser/OpenGEX_Parser.h>
#include <ManifestPersistence/BuildTool.h>
#include <ManifestUtility/Console_Color.h>
#include <ManifestPersistence/Manifest_DatabaseBuilder.h>

#include <ManifestMemory/Manifest_Allocator.h>
#include <EXPERIMENTAL/EXPERIMENTAL_RUNTIME_DATA_STRUCTURES.h>
#include <ManifestCommunication/EventSpace.h>

//currently testing terrain exports
#include <ManifestParser/ManifestDataDescriptor.h>
#include <ManifestTerrainCOPY/Voxel.h>

#include <thread>
#include <chrono>

using namespace Manifest_Parser;
using namespace Manifest_Persistence;
using namespace Manifest_Memory;
using namespace Manifest_Experimental;
using namespace Manifest_Communication;
using namespace Manifest_Terrain;

//MDD = MANIFEST DATA DESCRIPTOR FILE
//MBD = MANIFEST BINARY DATABASE
//MDB = MANIFEST DATABASE BUILDER STRUCTURE

const std::string TEST_PATH{ "D:\\Users\\Droll\\Desktop\\Game\\testing\\" };
const std::string TEST_GEX{ "Scene00.mdd" };
const std::string TEST_TERRAIN{ "Terrain Files\\0Terrain.mdd" };
const std::string TEST_VOXELMAP{ "Terrain Files\\0map.mdd" };
const std::string TEST_MBD{ "Test1.mbd" };
const std::string TEST_WORLDMAP{ "0Map.mbd" };

void PrintInfo(const DDL_Structure& structure)
{
	DLOG(33, "Structure " << structure.name << " with type: " << structure.identifier << " contains: " << structure.subSutructres.size() << " substructures");
	for (const auto& substructure : structure.subSutructres)
		PrintInfo(*substructure);
};
	
void ImportAndTestResourceDatabase()
{

	std::ifstream bImport{ TEST_PATH + TEST_MBD, std::ios::in | std::ios::binary };
	auto nLoops = 100000000;
	nLoops = 1;
	for(int i = 0 ; i < nLoops;++i)
	{
		ManifestBinaryResourceDatabase binaryDatabase = ImportGameResources(bImport);
		DLOG(45, "Printing Imported Database:");
	for (auto i = 0; i < binaryDatabase.binaryGeometryNodeTable.header.totalEntries; ++i)
	{
		const auto importNode = binaryDatabase.binaryGeometryNodeTable[i];
		const auto importGeometry = binaryDatabase.binaryGeometryObjectTable[importNode.header.geometryID];
		const auto importMesh = binaryDatabase.binaryMeshTable[importGeometry.header.meshID];
		const auto importMaterial = binaryDatabase.binaryMaterialTable[importNode.header.materialID];
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
		DLOG(31, "Material: " << importMaterial.header.materialID << " MTL(Diffuse): " << importMaterial.header.diffuseID << " MTL(Normal) : " << importMaterial.header.normalID << " MTL(Parllax) : " << importMaterial.header.parallaxID);
		DLOG(35, "Texture info - w: " << importTexture.header.width << " h: " << importTexture.header.height << " nChannels: " << +importTexture.header.nChannels << " size: " << importTexture.header.payloadSize);		
		DLOG(33,"Texture Data: ");	
		auto nTextureElements{ importTexture.header.payloadSize};
		for (auto data{ 0 }; data < nTextureElements; ++data)
			std::cout << +importTexture.payload[data] << ", ";
		std::cout << std::endl;
		DLOG(32, "Worldspace Data: ");
		for (auto index{ 0 };index <16;++index)
			std::cout << importNode.payload[index] << ", ";
		std::cout << std::endl;					
	}
	for (auto i = 0; i < binaryDatabase.binaryRigidBodyTable.header.totalEntries; ++i)
	{
		const auto& rigidBodies{ binaryDatabase.binaryRigidBodyTable[i] };
		DLOG(34, "number rigid bodies: " << rigidBodies.header.bodyCount);
		for (auto body{ 0 }; body < rigidBodies.header.bodyCount; ++body)
		{
			auto& payload{ rigidBodies.payload };			
			DLOG(34 + (body % 3), "Body.orientation: " << reinterpret_cast<const MFquaternion&>(rigidBodies.payload[sizeof(MFquaternion) * body]));			
			DLOG(34 + (body % 3), "Body.position: " << reinterpret_cast<const MFpoint3&>(rigidBodies.payload[rigidBodies.header.positionOffset + sizeof(MFpoint3) * body]));			
			DLOG(34 + (body % 3), "Body.uuid: " << reinterpret_cast<const MFu64&>(rigidBodies.payload[rigidBodies.header.objectIDOffset+ sizeof(MFpoint3) * body]));
		}
	}
	bImport.seekg(std::ios::beg);
	ScratchPad<Byte>{}.Unwind();
	}
}
void BuildAndExportResourceDatabase()
{
	//for printing purposes
	auto fileName = TEST_PATH + TEST_GEX;	
	auto file = LoadFileContents(fileName);
	DLOG(31, file);
	auto filtered = FilterFile(file);
	DLOG(32, filtered);	
	auto fileContents = PartitionDDLFile(filtered);
	for (const auto& c : fileContents)
		DLOG(33, c);
	//begin actual parse	
	auto nLoops = 1950000;	
	nLoops = 1;
	for (auto loop = 0; loop < nLoops; ++loop)
	{			
		{			
			DDL_File fileObject;
			//performs load and filter and begins parse - writes to scratch pad allocator - must unwind when finished with parsed data
			ParseDDLFile(fileContents, fileObject);
			//prints primary and substructure information per top level sturcture

			//build offline database	
			
			ManifestResourceDatabaseBuilder databaseBuilder;
			BuildResourceDatabase(fileObject, databaseBuilder);

			std::ofstream bExport{ TEST_PATH + TEST_MBD, std::ios::out | std::ios::binary };
			if (bExport.is_open())
			{
				DLOG(45, "Printing Exported Database:");
				ExportBinaryResourceDatabase(databaseBuilder, bExport);
				bExport.close();
			}
		}
		ScratchPad<Byte>{}.Unwind();
	}			
}

void ImportAndTestWorldDatabase()
{
	auto fileName = TEST_PATH + TEST_WORLDMAP;
	std::ifstream bImport{ fileName, std::ios::in | std::ios::binary };
	auto nLoops = 100000000;
	nLoops = 1;
	for (int i = 0; i < nLoops; ++i)
	{
		ManifestBinaryWorldDatabase binaryDatabase = ImportGameTerrain(bImport);
		DLOG(45, "Printing Imported Database:");
		for (auto i = 0; i < binaryDatabase.binaryTerrainTable.header.totalEntries; ++i)
		{
			const auto& importTerrain = binaryDatabase.binaryTerrainTable[i];
			DLOG(31, "Reading Terrain Chunk: " << i);
			const auto& index = importTerrain.header.terrainHash;
			DLOG(32, "Terrain index hash: " << index);
			DLOG(32, "Terrain xIndex: " << GetCompositeWard(index,TERRAIN_Z_INDEX_HASH_OFFSET) << " zIndex: " << GetCompositeBow(index, TERRAIN_Z_INDEX_HASH_OFFSET));		
		}
		for (auto i = 0; i < binaryDatabase.binaryVoxelMapTable.header.totalEntries; ++i)
		{
			const auto& importVoxelMap = binaryDatabase.binaryVoxelMapTable[i];
			DLOG(31, "Reading Voxel Map: " << i);
			DLOG(32, " xChunks: " << importVoxelMap.header.xChunks << " zChunks: " << importVoxelMap.header.zChunks);
			DLOG(33, " nVoxels: " << importVoxelMap.header.nVoxels << " mVoxels: " << importVoxelMap.header.mVoxels << " hVoxels: " << importVoxelMap.header.hVoxels);
			DLOG(37, "SDF: ");
			for (auto sample{ 0 }; sample < importVoxelMap.header.payloadSize; ++sample)
				std::cout << +importVoxelMap.payload[sample] << ",";
		}		
	}
}
void BuildAndExportWorldDatabase(std::vector<std::string> filenames)
{	
	DDL_File fileObject;
	//load ddl files into file object	
	for (const auto& filename : filenames)
	{
		auto fileContents = LoadFileContents(filename);		
		auto filteredFile = FilterFile(fileContents);		
		auto partitionedFile = PartitionDDLFile(filteredFile);		
		ParseDDLFile(partitionedFile, fileObject);
	}	
	
	//build offline database	
	ManifestWorldDatabaseBuilder databaseBuilder;
	BuildWorldDatabase(fileObject, databaseBuilder);
	
	//export binary database	
	auto fileName = TEST_PATH + TEST_WORLDMAP;
	std::ofstream bExport{ fileName, std::ios::out | std::ios::binary };
	if (bExport.is_open())
	{
		DLOG(45, "Printing Exported Database:");
		ExportBinaryTerrainDatabase(databaseBuilder, bExport);
		bExport.close();
	}		
}

using Seconds = std::chrono::duration<double>;
using Milliseconds = std::chrono::duration<double, std::milli>;
using Nanoseconds = std::chrono::duration<double, std::nano>;
using Timepoint = std::chrono::time_point<std::chrono::steady_clock,Nanoseconds>;

void CreateWorldMDD(MFbool buildMDDs)
{
	auto func{ 0 };
	auto seed{ 0 };
	auto nChunks{ 1 };
	auto mChunks{ 1 };
	auto hChunks{ 1 };
	auto lod{ 1 };
	std::cout << "nChunks: ";
	std::cin >> nChunks;
	std::cout << "mChunks: ";
	std::cin >> mChunks;
	std::cout << "Max LOD(0-3): ";
	std::cin >> lod;
	std::cout << "seed: ";
	std::cin >> seed;
	std::cout << "Density Function(0-3): ";
	std::cin >> func;
	auto map{ GenerateVoxelMap(0,lod,nChunks,mChunks,hChunks,func) };
	if (buildMDDs)
	{
		ExportVoxelMapMDD(TEST_PATH + TEST_VOXELMAP, nChunks, mChunks, hChunks, lod, map.field);
		//currently unused
		ExportTerrainMDD(TEST_PATH + TEST_TERRAIN, nChunks, mChunks, hChunks, lod, map.field);
	}
	std::vector<std::string> mddFilenames
	{ TEST_PATH + TEST_VOXELMAP,TEST_PATH + TEST_TERRAIN };
	BuildAndExportWorldDatabase(mddFilenames);
	ScratchPad<Byte>{}.Unwind();
}

int main()
{		
	RegisterProgramExecutiveThread();
	//create data stores
	INIT_MEMORY_RESERVES();	
	MEMORYSTATUSEX status;
	status.dwLength = sizeof(status);
	GlobalMemoryStatusEx(&status);
	
	//ddl start up
	Initialize_GEXTypes();
	Initialize_GEXGenerators();

	//persistence tests
	DISABLE
		CreateWorldMDD(true);
	DISABLE
		ImportAndTestWorldDatabase();
	//DISABLE
		BuildAndExportResourceDatabase();
	//DISABLE
		ImportAndTestResourceDatabase();	
	
	return 0;
}

