#pragma once

#include <ManifestParser/OpenGEX_Parser.h>
#include <ManifestPersistence/BuildTool.h>
#include <ManifestUtility/Console_Color.h>
#include <ManifestPersistence/Manifest_DatabaseBuilder.h>
#include <ManifestPersistence/Runtime_Database.h>

#include <ManifestMemory/Manifest_Allocator.h>
#include <EXPERIMENTAL/EXPERIMENTAL_RUNTIME_DATA_STRUCTURES.h>
#include <ManifestCommunication/EventSpace.h>

#include <thread>
#include <chrono>

using namespace Manifest_Parser;
using namespace Manifest_Persistence;
using namespace Manifest_Memory;
using namespace Manifest_Experimental;
using namespace Manifest_Communication;

const std::string TEST_PATH{ "C:\\Users\\Droll\\Desktop\\Game\\testing\\" };
const std::string TEST_GEX{ "Test2.gex" };
const std::string TEST_MDB{ "Test2.mdb" };

void RuntimeTest()
{
	
} 

void PrintInfo(const DDL_Structure& structure)
{
	DLOG(33, "Structure " << structure.name << " with type: " << structure.identifier << " contains: " << structure.subSutructres.size() << " substructures");
	for (const auto& substructure : structure.subSutructres)
		PrintInfo(*substructure);
};
	
void ImportAndTest()
{

	std::ifstream bImport{ TEST_PATH + TEST_MDB, std::ios::in | std::ios::binary };
	auto nLoops = 100000000;
	nLoops = 1;
	for(int i = 0 ; i < nLoops;++i)
	{
		ManifestBinaryDatabase binaryDatabase = ImportBinaryDatabase(bImport);	
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
		auto nTextureElements{ importTexture.header.payloadSize / sizeof(MFfloat)};
		for (auto data{ 0 }; data < nTextureElements; ++data)
			std::cout << importTexture.payload[data] << ", ";
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

void BuildAndExport()
{
	//for printing purposes
	auto file = LoadFileContents(TEST_PATH + TEST_GEX);
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
			
			ManifestDatabaseBuilder databaseBuilder;
			BuildOfflineDatabase(fileObject, databaseBuilder);		
			std::ofstream bExport{ TEST_PATH + TEST_MDB, std::ios::out | std::ios::binary };			
			if (bExport.is_open())
			{
				DLOG(45, "Printing Exported Database:");
				ExportBinaryDatabase(databaseBuilder, bExport);
				bExport.close();
			}
		}
		ScratchPad<Byte>{}.Unwind();
	}			
}

template<typename T>
struct Deleter
{
	void operator()(T* p) const
	{
		//DLOG(31, "Deleting int*: " << p << " with value: " << *p);
		delete p;
	}
};

using stdIntDelete = Deleter<int>;

#include <ManifestMemory/MemoryGuards/RCU.h>

using intRCU2 = RCU<int, stdIntDelete>;

int globalInt{ 0 };

void DoSomethingWithInt(const int& myInt, int& read)
{
	++read;
}

using Seconds = std::chrono::duration<double>;
using Milliseconds = std::chrono::duration<double, std::milli>;
using Nanoseconds = std::chrono::duration<double, std::nano>;
using Timepoint = std::chrono::time_point<std::chrono::steady_clock,Nanoseconds>;

struct paddedInt
{
	int i;
	char padding[64 - sizeof(int)];
};

Table<int, std::default_delete<int>> intTable{ 3 };
Table<Texture, std::default_delete<Texture>> textureTable{ 3 };
Table<Material, std::default_delete<Material>> materialTable{ 3 };

template<typename T, typename Deleter>
void MyReadFunc(const typename RCU<T, Deleter>::Handle& generationHandle, T* copyReadToAddress)
{
	*copyReadToAddress = *generationHandle.handle;
}

template<typename T, typename... Params>
T* MyWriteFunc(const Params& ...data)
{
	return new T{ data... };
}

void readfunc(const Timepoint& begin, const Timepoint& end, int& read)
{
	auto readerID = intTable.ReserveTableReadFlag();
	std::this_thread::sleep_until(begin);
	while (std::chrono::high_resolution_clock::now() < end)
	{
		int copyInt;
		intTable.Pull(readerID,MyReadFunc<int,std::default_delete<int>>,&copyInt);
		std::this_thread::sleep_for(Milliseconds{ 6.95 });
		DLOG(34, "Pulled int: " << copyInt);
		++read;		
	}
}

void writefunc(const Timepoint& begin, const Timepoint& end)
{
	std::this_thread::sleep_until(begin);
	while (std::chrono::high_resolution_clock::now() < end)
	{
		intTable.Push(MyWriteFunc<int,int>, ++globalInt);
		std::this_thread::sleep_for(Milliseconds{ 16.6 });
	}
}


int main()
{	
	auto loops = 0;
	for (auto loop{ 0 }; loop < loops; loop+=1)
	{		
		Seconds beginDelay{ .1 };
		Seconds executionTime{ 1 };
		Timepoint beginTime = std::chrono::high_resolution_clock::now() + beginDelay;
		Timepoint endTime = beginTime + executionTime;
		paddedInt reads[6];
		for (auto& read : reads)
			read.i = 0;
		std::thread rthread1{ readfunc,std::cref(beginTime),std::cref(endTime),std::ref(reads[0].i)};
		//std::thread rthread2{ readfunc,std::cref(beginTime),std::cref(endTime),std::ref(reads[1].i)};
		//std::thread rthread3{ readfunc,std::cref(beginTime),std::cref(endTime),std::ref(reads[2].i)};
		writefunc(beginTime, endTime);
		rthread1.join();
		//rthread2.join();
		//rthread3.join();
		auto wAvg = globalInt / executionTime.count();
		int readInt{ 0 };
		for (const auto& read : reads)
			readInt += read.i;
		auto rAvg = readInt / executionTime.count();
		//LOG(32, "Avg writes/s: " << wAvg);
		LOG(33, "Avg reads/s: " << rAvg << " executionTime: " << executionTime);
		globalInt = 0;
	}

	RegisterProgramExecutiveThread();
	//create data stores
	INIT_MEMORY_RESERVES();	
	MEMORYSTATUSEX status;
	status.dwLength = sizeof(status);
	GlobalMemoryStatusEx(&status);

	//persistence tests
	//DISABLE
		BuildAndExport();
	//DISABLE
		ImportAndTest();
	//final
	DISABLE
		RuntimeTest();
	
	std::getchar();
	return 0;
}

