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

const std::string TEST_PATH{ "C:\\Users\\Droll\\Desktop\\Game\\testing\\" };
const std::string TEST_GEX{ "Test2.gex" };
const std::string TEST_TERRAIN{ "Terrain Files\\0Terrain.mdd" };
const std::string TEST_VOXELMAP{ "Terrain Files\\0map.mdd" };
const std::string TEST_MBD{ "Test2.mbd" };
const std::string TEST_TERRAIN_BINARY{ "Terrain Files\\0Terrain.mbd" };

void RuntimeTest()
{
	
} 

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
	bImport.seekg(std::ios::beg);
	ScratchPad<Byte>{}.Unwind();
	}
}

void BuildAndExportResourceDatabase()
{
	//for printing purposes
	auto fileName = TEST_PATH + TEST_GEX;
	//fileName.insert(fileName.find_first_of('.'), std::to_string(0));
	auto file = LoadFileContents(fileName);
	//DLOG(31, file);
	auto filtered = FilterFile(file);
	//DLOG(32, filtered);	
	auto fileContents = PartitionDDLFile(filtered);
	//begin actual parse
	//ddl start up
	Initialize_GEXTypes();
	Initialize_GEXGenerators();	
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

void ImportAndTestTerrainDatabase()
{
	auto fileName = TEST_PATH + TEST_TERRAIN_BINARY;
	fileName.insert(fileName.find_first_of('.'), std::to_string(0));
	std::ifstream bImport{ fileName, std::ios::in | std::ios::binary };
	auto nLoops = 100000000;
	nLoops = 1;
	for (int i = 0; i < nLoops; ++i)
	{
		ManifestBinaryTerrainDatabase binaryDatabase = ImportGameTerrain(bImport);
		DLOG(45, "Printing Imported Database:");
		for (auto i = 0; i < binaryDatabase.binaryTerrainTable.header.totalEntries; ++i)
		{
			const auto& importTerrain = binaryDatabase.binaryTerrainTable[i];
			DLOG(31, "Reading Terrain Chunk: " << i);
			const auto& index = importTerrain.header.terrainHash;
			DLOG(32, "Terrain index hash: " << index);
			DLOG(32, "Terrain xIndex: " << GetCompositeWard(index,TERRAIN_Z_INDEX_HASH_OFFSET) << " zIndex: " << GetCompositeBow(index, TERRAIN_Z_INDEX_HASH_OFFSET));
			DLOG(32, "Chunk SDF: ");
			for (auto index{ 0 }; index < importTerrain.header.payloadSize; ++index)
				std::cout << +importTerrain.payload[index] << ", ";
		}
	}
}
void BuildAndExportTerrainDatabase()
{
	//for printing purposes
	auto fileName = TEST_PATH + TEST_TERRAIN;
	fileName.insert(fileName.find_first_of('.'), std::to_string(0));
	auto file = LoadFileContents(fileName);
	//DLOG(31, file);
	auto filtered = FilterFile(file);
	//DLOG(32, filtered);	
	auto fileContents = PartitionDDLFile(filtered);
	//begin actual parse
	//ddl start up
	Initialize_GEXTypes();
	Initialize_GEXGenerators();
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

			ManifestTerrainDatabaseBuilder databaseBuilder;
			BuildTerrainDatabase(fileObject, databaseBuilder);

			auto fileName = TEST_PATH + TEST_TERRAIN_BINARY;
			fileName.insert(fileName.find_first_of('.'), std::to_string(0));
			std::ofstream bExport{ fileName, std::ios::out | std::ios::binary };
			if (bExport.is_open())
			{
				DLOG(45, "Printing Exported Database:");
				ExportBinaryTerrainDatabase(databaseBuilder, bExport);
				bExport.close();
			}
		}
		ScratchPad<Byte>{}.Unwind();
	}
}

using Seconds = std::chrono::duration<double>;
using Milliseconds = std::chrono::duration<double, std::milli>;
using Nanoseconds = std::chrono::duration<double, std::nano>;
using Timepoint = std::chrono::time_point<std::chrono::steady_clock,Nanoseconds>;

void CreateTerrainMDD()
{
	auto nBlocks{ 1 };
	auto mBlocks{ 1 };
	auto hBlocks{ 1 };
	auto lod{ 0 };
	auto map{ GenerateVoxelMap(0,lod,nBlocks,mBlocks,hBlocks,{0}) };
	const MFu32& nVoxels = (19 + (nBlocks - 1) * 17) << lod;
	const MFu32& mVoxels = (19 + (mBlocks - 1) * 17) << lod;
	const MFu32& hVoxels = (19 + (hBlocks - 1) * 17) << lod;	
	const auto voxelCount{ nVoxels + mVoxels + hVoxels };	
	ExportVoxelMapMDD(TEST_PATH + TEST_VOXELMAP, nBlocks, mBlocks, hBlocks, lod, map.field);
	ExportTerrainMDD(TEST_PATH + TEST_TERRAIN, nBlocks, mBlocks, hBlocks, lod, map.field);	
	BuildAndExportTerrainDatabase();
}

int main()
{		

	RegisterProgramExecutiveThread();
	//create data stores
	INIT_MEMORY_RESERVES();	
	MEMORYSTATUSEX status;
	status.dwLength = sizeof(status);
	GlobalMemoryStatusEx(&status);

	//persistence tests
	//DISABLE
		CreateTerrainMDD();
	//DISABLE
		ImportAndTestTerrainDatabase();
	DISABLE
		BuildAndExportResourceDatabase();
	DISABLE
		ImportAndTestResourceDatabase();
	//final
	DISABLE
		RuntimeTest();
	
	return 0;
}

