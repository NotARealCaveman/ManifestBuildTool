#pragma once

#include <ManifestGLParser/OpenGEX_Parser.h>
#include <ManifestGLPersistence/BuildTool.h>
#include <ManifestGLUtility/Console_Color.h>
#include <ManifestGLPersistence/Manifest_DatabaseBuilder.h>
#include <ManifestGLPersistence/Runtime_Database.h>

#include <EXPERIMENTAL/Manifest_Allocator.h>
#include <EXPERIMENTAL/EXPERIMENTAL_RUNTIME_DATA_STRUCTURES.h>
#include <EXPERIMENTAL/ManifestCommunication/FileSystem.h>

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
	//unwind allocations from binary import
	ScratchPad<Byte>{}.Unwind();

	auto geometryObjects = runtimeDatabase.PullGeometryObjects();
	auto geometryNodes = runtimeDatabase.PullGeometryNodes();
	auto materials = runtimeDatabase.PullMaterials();

	for (auto nodeEntry = 0; nodeEntry < geometryNodes->geometryNodeTable.tableSize*0; ++nodeEntry)
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

void ThreadTest()
{
	std::ifstream bImport{ TEST_PATH + TEST_MDB, std::ios::in | std::ios::binary };
	ManifestRuntimeDatabase runtimeDatabase{ ImportBinaryDatabase(bImport) };
	std::thread rthread{ RenderThread,std::ref(runtimeDatabase) };	
	runtimeDatabase.simThreadId = std::this_thread::get_id();
	runtimeDatabase.renderThreadId = rthread.get_id();
	//std::thread mThread{ MessageThread };
	SimThread(runtimeDatabase);//runs on main thread	
	rthread.join();		
}


void MessageTest()
{

	//set up event space and observers	
	FileSystemEventSpace fsEventSpace;
	FileSystemObservationToken fsToken{ UnderlyingType(FileSystemMessageType::MBD_MATERIAL | FileSystemMessageType::MBD_TEXTURE) };
	FileSystemObserver fsObserver{ fsToken,fsEventSpace.observerRegister };
	//create observed system
	FileSystem fileSystem;
	//load database and send fs messages
	fileSystem.LoadMBD(TEST_PATH + TEST_MDB, fsEventSpace);
	auto processMessage = fsObserver.ProcessEvents(TEST_PROCESS_FUNC);
	//push commited information to database


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

#include <EXPERIMENTAL/RCU.h>

using intRCU = RCU<int, stdIntDelete>;
intRCU rcu;
using intRCU2 = TableRCU<int, stdIntDelete>;
intRCU2 rcu2(6);
int globalInt{ 0 };

void DoSomethingWithInt(const int& myInt, int& read)
{
	++read;
}

using Seconds = std::chrono::duration<double>;
using Milliseconds = std::chrono::duration<double, std::milli>;
using Nanoseconds = std::chrono::duration<double, std::nano>;
using Timepoint = std::chrono::time_point<std::chrono::steady_clock,Nanoseconds>;

void readfunc(const Timepoint& begin, const Timepoint& end, int& read)
{
	auto readerID = rcu2.RegisterReader();
	std::this_thread::sleep_until(begin);
	while (std::chrono::high_resolution_clock::now() < end)		
	{
		auto readGeneration = rcu2.lock(readerID);
		++read;
		rcu2.unlock(readGeneration, readerID);
	}
}

void writefunc(const Timepoint& begin, const Timepoint& end)
{
	std::this_thread::sleep_until(begin);
	while (std::chrono::high_resolution_clock::now() < end)		
	{
		auto newData = new int{ ++globalInt };
		rcu.synchronize_rcu(newData);				
		//std::this_thread::sleep_for(Milliseconds{ 1 });
	}	
}

struct paddedInt
{
	int i;
	char padding[64 - sizeof(int)];
};

int main()
{		
	constexpr auto size = sizeof(intRCU2::DataGeneration<int>);
	auto loops = 200;
	for (auto loop{ 0 }; loop = loops; loop+=1)
	{
		Seconds beginDelay{ .1 };
		Seconds executionTime{ 1 };
		Timepoint beginTime = std::chrono::high_resolution_clock::now() + beginDelay;
		Timepoint endTime = beginTime + executionTime;
		paddedInt reads[6];
		for (auto& read : reads)
			read.i = 0;
		std::thread rthread1{ readfunc,std::cref(beginTime),std::cref(endTime),std::ref(reads[0].i) };
		std::thread rthread2{ readfunc,std::cref(beginTime),std::cref(endTime),std::ref(reads[1].i) };
		std::thread rthread3{ readfunc,std::cref(beginTime),std::cref(endTime),std::ref(reads[2].i) };
		std::thread rthread4{ readfunc,std::cref(beginTime),std::cref(endTime),std::ref(reads[3].i) };
		std::thread rthread5{ readfunc,std::cref(beginTime),std::cref(endTime),std::ref(reads[4].i) };
		std::thread rthread6{ readfunc,std::cref(beginTime),std::cref(endTime),std::ref(reads[5].i) };
		writefunc(beginTime, endTime);
		rthread1.join();
		rthread2.join();
		rthread3.join();
		rthread4.join();
		rthread5.join();
		rthread6.join();
		auto wAvg = globalInt / executionTime.count();
		int readInt{ 0 };
		for (const auto& read : reads)
			readInt += read.i;
		auto rAvg = readInt / executionTime.count();
		//LOG(32, "Avg writes/s: " << wAvg);
		LOG(33, "Avg reads/s: " << rAvg << " executionTime: " << executionTime);
		globalInt = 0;
		rcu2.registeredReaders = 0;		
	}
	RegisterProgramExecutiveThread();
	//create data stores
	INIT_MEMORY_RESERVES();	
	MEMORYSTATUSEX status;
	status.dwLength = sizeof(status);
	GlobalMemoryStatusEx(&status);

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
	
	std::getchar();
	return 0;
}
