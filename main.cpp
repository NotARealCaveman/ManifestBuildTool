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
	WorldSpaces worldSpaces;
	worldSpaces.xforms.tableSize = nNodes;
	worldSpaces.xforms.tableEntries = 0;
	worldSpaces.xforms.keys = new UniqueKey[nNodes];
	worldSpaces.xforms.values = new Xform[nNodes];

	std::ifstream bImport{ TEST_PATH + TEST_MDB, std::ios::in | std::ios::binary };	
	ManifestRuntimeDatabase runtimeDatabase{ ImportBinaryDatabase(bImport), worldSpaces };

	
}



void PrintInfo(const DDL_Structure& structure)
{
	DLOG(33, "Structure " << structure.name << " with type: " << structure.identifier << " contains: " << structure.subSutructres.size() << " substructures");
	for (const auto& substructure : structure.subSutructres)
		PrintInfo(substructure);
};

void BuildAndExport()
{
	auto file = LoadFileContents(TEST_PATH + TEST_GEX);
	DLOG(31, file);
	auto filtered = FilterFile(file);
	DLOG(32, filtered);

	//ddl start up
	Initialize_GEXTypes();
	Initialize_GEXGenerators();
	//parse
	DDL_File fileObject;	

	ParseDDLFile("", fileObject);

	for (const auto& primary : fileObject.primaryStructures)
	{
		PrintInfo(primary);
		DLOG(31,"");
	}

	//build offline database
	ManifestDatabaseBuilder databaseBuilder;
	BuildOfflineDatabase(fileObject, databaseBuilder);
	//export conversion

	//export
	std::ofstream bExport{ TEST_PATH+TEST_MDB, std::ios::out | std::ios::binary };
	if (bExport.is_open())
	{
		ExportBinaryDatabase(databaseBuilder, bExport);
		bExport.close();
	}
}
	
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

	}
}

int main()
{
	WINDOWS_COLOR_CONSOLE;	

	DISABLE
		BuildAndExport();
	DISABLE
		ImportAndTest();
	//DISABLE
		RuntimeTest();
	
	return 0;
}