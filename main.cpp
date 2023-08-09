#pragma once

#include <ManifestGLFramework/ManifestMath/Vector2.h>

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
	for (auto i = 0; i < binaryDatabase.binaryDynamicRigidBodyTable.header.totalEntries; ++i)
	{
		const auto& rigidBodies{ binaryDatabase.binaryDynamicRigidBodyTable[i] };
		DLOG(34, "number dynamic rigid bodies: " << rigidBodies.header.bodyCount);
		for (auto body{ 0 }; body < rigidBodies.header.bodyCount; ++body)
		{
			auto& payload{ rigidBodies.payload };			
			DLOG(34 + (body % 3), "Body.orientation: " << reinterpret_cast<const MFquaternion&>(rigidBodies.payload[sizeof(MFquaternion) * body]));			
			DLOG(34 + (body % 3), "Body.position: " << reinterpret_cast<const MFpoint3&>(rigidBodies.payload[rigidBodies.header.positionOffset + sizeof(MFpoint3) * body]));			
			DLOG(34 + (body % 3), "Body.uuid: " << reinterpret_cast<const MFu64&>(rigidBodies.payload[rigidBodies.header.objectIDOffset+ sizeof(MFpoint3) * body]));
		}
	}
	for (auto i = 0; i < binaryDatabase.binaryStaticRigidBodyTable.header.totalEntries; ++i)
	{
		const auto& rigidBodies{ binaryDatabase.binaryStaticRigidBodyTable[i] };
		DLOG(34, "number static rigid bodies: " << rigidBodies.header.bodyCount);
		for (auto body{ 0 }; body < rigidBodies.header.bodyCount; ++body)
		{
			auto& payload{ rigidBodies.payload };
			DLOG(34 + (body % 3), "Body.orientation: " << reinterpret_cast<const MFquaternion&>(rigidBodies.payload[sizeof(MFquaternion) * body]));
			DLOG(34 + (body % 3), "Body.position: " << reinterpret_cast<const MFpoint3&>(rigidBodies.payload[rigidBodies.header.positionOffset + sizeof(MFpoint3) * body]));
			DLOG(34 + (body % 3), "Body.uuid: " << reinterpret_cast<const MFu64&>(rigidBodies.payload[rigidBodies.header.objectIDOffset + sizeof(MFpoint3) * body]));
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

void OBJtoGEX(const std::string& objFile)
{
	std::vector<MFvec3> vertices;
	std::vector<MFvec2> uvs;
	std::vector<MFvec3> normals;
	auto file = LoadFileContents(objFile);
	DLOG(34, file);
	std::stringstream fileStream{ file };
	std::string current;
	std::getline(fileStream, current);
	while (current[0] != 'v')
		std::getline(fileStream, current);
	while (current[0] == 'v' && current[1] != 't' && current[1] != 'n')
	{
		std::stringstream vertexStream{ current.substr(2) };
		MFvec3 vertex;
		std::string value;
		for (auto axis{ 0 }; axis < 3; ++axis)
		{
			vertexStream >> value;
			vertex[axis] = std::stof(value);
		}
		vertices.emplace_back(vertex);
		std::getline(fileStream, current);
	}
	while (current[0] == 'v' && current[1] == 't')
	{
		std::stringstream vertexStream{ current.substr(3) };
		MFvec2 uv;
		std::string value;
		for (auto axis{ 0 }; axis < 2; ++axis)
		{
			vertexStream >> value;
			uv[axis] = std::stof(value);
		}
		uvs.emplace_back(uv);
		std::getline(fileStream, current);
	}
	while (current[0] == 'v' && current[1] == 'n')
	{
		std::stringstream vertexStream{ current.substr(3) };
		MFvec3 normal;
		std::string value;
		for (auto axis{ 0 }; axis < 3; ++axis)
		{
			vertexStream >> value;
			normal[axis] = std::stof(value);
		}
		normals.emplace_back(normal);
		std::getline(fileStream, current);
	}
	while (current[0] != 'f')
		std::getline(fileStream, current);
	struct OBJface
	{
		MFu32 vertices[3];
		MFu32 uvs[3];
		MFu32 normals[3];
	};
	std::vector<OBJface> objFaces;
	while (current[0] == 'f')
	{
		std::string indices{ current.substr(2).data()};
		OBJface face;
		for (auto point{ 0 }; point < 3; ++point)
		{
			std::string list{ indices.substr(0,indices.find_first_of(' ')) };
			for (auto value{ 0 }; value < 3; ++value)
			{
				std::string index{ list.substr(0,list.find_first_of('/')) };
				if (index.empty())
				{
					list = list.substr(list.find_first_of('/') + 1);
					continue;
				}
				switch (value)
				{
				case 0:
					face.vertices[point] = std::stoi(index);
					break;
				case 1:
					face.uvs[point] = std::stoi(index);
					break;
				case 2:
					face.normals[point] = std::stoi(index);
					break;
				}
				list = list.substr(list.find_first_of('/') + 1);
			}
			indices = indices.substr(indices.find_first_of(' ') + 1);
		}
		objFaces.emplace_back(face);
		std::getline(fileStream, current);
	}
	//prepare obj data into render format
	std::vector<MFvec3> finalVertices;
	std::vector<MFvec2> finalUVs;
	std::vector<MFvec3> finalNormals;
	std::vector<MFu32> finalIndices;
	MFu32 index{ 0 };
	for (const auto& face : objFaces)
	{
		if (vertices.size())
		{
			finalVertices.emplace_back(vertices[face.vertices[0] - 1]);
			finalVertices.emplace_back(vertices[face.vertices[1] - 1]);
			finalVertices.emplace_back(vertices[face.vertices[2] - 1]);
		}
		if (uvs.size())
		{
			finalUVs.emplace_back(uvs[face.uvs[0] - 1]);
			finalUVs.emplace_back(uvs[face.uvs[1] - 1]);
			finalUVs.emplace_back(uvs[face.uvs[2] - 1]);
		}
		if (normals.size())
		{
			finalNormals.emplace_back(normals[face.normals[0] - 1]);
			finalNormals.emplace_back(normals[face.normals[1] - 1]);
			finalNormals.emplace_back(normals[face.normals[2] - 1]);
		}		
		finalIndices.emplace_back(index++);
		finalIndices.emplace_back(index++);
		finalIndices.emplace_back(index++);
	}		
	//convert render format to opengex vertex arrays
	std::ofstream gexFile{ TEST_PATH + "Capsule.gex" };
	std::string line{ "GeometryObject $Capsule //Capsule\n" };
	gexFile.write(line.c_str(), line.size());
	line = "{\n\tMesh (primitive = \"triangles\", lod=0)\n";	
	gexFile.write(line.c_str(), line.size());
	//write position data
	line = "\t{\n\t\tVertexArray (attrib = \"position\")\n";
	gexFile.write(line.c_str(), line.size());
	line = "\t\t{\n\t\t\tfloat[3]\n\t\t\t{\n\t\t\t\t";
	gexFile.write(line.c_str(), line.size());	
	int writeCount = 0;
	for (const auto& vertex : finalVertices)
	{
		++writeCount;
		line = "{ " + std::to_string(vertex.x) + ", " + std::to_string(vertex.y) + ", " + std::to_string(vertex.z) + "} ";
		if (&vertex != finalVertices.data() + finalVertices.size() - 1)
			line += ", ";
		if (!(writeCount % 100))
			line += "\n\t\t\t\t";
		gexFile.write(line.c_str(), line.size());		
	}
	DLOG(34, writeCount << " vertices written");
	line = "\n\t\t\t}\n\t\t}";
	gexFile.write(line.c_str(), line.size());
	//write normal data
	line = "\n\t\tVertexArray (attrib = \"normal\")\n";
	gexFile.write(line.c_str(), line.size());
	line = "\t\t{\n\t\t\tfloat[3]\n\t\t\t{\n\t\t\t\t";
	gexFile.write(line.c_str(), line.size());
	writeCount = 0;
	for (const auto& normal: finalNormals)
	{
		++writeCount;
		line = "{ " + std::to_string(normal.x) + ", " + std::to_string(normal.y) + ", " + std::to_string(normal.z) + "} ";
		if (&normal != finalNormals.data() + finalNormals.size() - 1)
			line += ", ";
		if (!(writeCount % 100))
			line += "\n\t\t\t\t";
		gexFile.write(line.c_str(), line.size());
	}
	DLOG(34, writeCount << " normals written");
	line = "\n\t\t\t}\n\t\t}";
	gexFile.write(line.c_str(), line.size());
	//write uv data
	line = "\n\t\tVertexArray (attrib = \"texcoord\")\n";
	gexFile.write(line.c_str(), line.size());
	line = "\t\t{\n\t\t\tfloat[2]\n\t\t\t{\n\t\t\t\t";
	gexFile.write(line.c_str(), line.size());
	writeCount = 0;	
	for (const auto& uv : finalUVs)
	{
		++writeCount;
		line = "{ " + std::to_string(uv.x) + ", " + std::to_string(uv.y) + "} ";
		if(&uv != finalUVs.data() + finalUVs.size() - 1)
			line += ", ";
		if (!(writeCount % 100))
			line += "\n\t\t\t\t";
		gexFile.write(line.c_str(), line.size());
	}
	DLOG(34, writeCount << " uvs written");
	line = "\n\t\t\t}\n\t\t}";
	gexFile.write(line.c_str(), line.size());
	//write index data
	line = "\n\t\IndexArray\n";
	gexFile.write(line.c_str(), line.size());
	line = "\t\t{\n\t\t\tuint32[3]\n\t\t\t{\n\t\t\t\t";
	gexFile.write(line.c_str(), line.size());
	writeCount = 0;
	for (auto index{ 0 }; index < finalIndices.size(); index+=3)
	{
		++writeCount;
		line = "{ " + std::to_string(finalIndices[index + 0]) + ", " +std::to_string(finalIndices[index + 1]) + ", " + std::to_string(finalIndices[index + 2]) + "}";
		if (index + 3 < finalIndices.size())
			line += ", ";
		if (!(writeCount % 100))
			line += "\n\t\t\t\t";
		gexFile.write(line.c_str(), line.size());
	}
	DLOG(34, writeCount << " indices written");
	line = "\n\t\t\t}\n\t\t}";
	gexFile.write(line.c_str(), line.size());
	//close up file
	line = "\n\t}\n}";
	gexFile.write(line.c_str(), line.size());	
	gexFile.close();
}

void CreateWorldMDD(MFbool buildMDDs)
{
	if (buildMDDs)
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

	
	DISABLE
	{		
		
		OBJtoGEX("C:\\Users\\Droll\\Desktop\\Game\\Reign\\Models\\Sphere.obj");
		OBJtoGEX("D:\\Users\\Droll\\Desktop\\Game\\Testing\\OBJ-_gex\\Capsule.obj");
	}


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

