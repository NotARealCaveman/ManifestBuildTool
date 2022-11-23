#pragma once

#include <ManifestGLParser/OpenGEX_Parser.h>
#include <ManifestGLPersistence/BuildTool.h>
#include <ManifestGLUtility/Console_Color.h>
#include <ManifestGLPersistence/MDB/Binary/Binary_Types.h>

using namespace Manifest_Parser;
using namespace Manifset_Persistence;

int main()
{	
	{
	WINDOWS_COLOR_CONSOLE
	//ddl start up
	Initialize_GEXTypes();
	Initialize_GEXGenerators();
	//parse
	DDL_File fileObject;

	ParseDDLFile("", fileObject);

	//build database
	ManifestDatabase database;
	BuildDatabase(fileObject, database);
	//export conversion
	const BinaryMeshTable exportTable{ BuildBinaryTable<Binary_Mesh,MeshTable,VertexTables,IndexTable>(database.meshTable,database.vertexTables,database.indexTable) };
	const BinaryGeometryObjectTable exportTable2{ BuildBinaryTable<Binary_GeometryObject,GeometryObjectTable>(database.geometryObjectTable) };
	const BinaryGeometryNodeTable exportTable3{ BuildBinaryTable<Binary_GeometryNode,GeometryNodeTable,ObjectRefTable,MaterialRefTable>(database.geometryNodeTable,database.objectRefTable,database.materialRefTable)};
	//export
	std::ofstream bExport{ "C:\\Users\\Droll\\Desktop\\Game\\testoimng\\TEST.mdb", std::ios::out | std::ios::binary };
	if (bExport.is_open())
	{
		ExportBinaryTable(exportTable, bExport);
		ExportBinaryTable(exportTable2, bExport);
		ExportBinaryTable(exportTable3, bExport);
		bExport.close();
	}
	//test import 
	std::ifstream bImport{ "C:\\Users\\Droll\\Desktop\\Game\\testoimng\\TEST.mdb", std::ios::in | std::ios::binary };
	BinaryMeshTable importTable;
	BinaryGeometryObjectTable importTable2;
	BinaryGeometryNodeTable importTable3;
	if (bImport.is_open())
	{
		importTable = ImportBinaryTable<Binary_Mesh>(bImport);
		importTable2 = ImportBinaryTable<Binary_GeometryObject>(bImport);
		importTable3 = ImportBinaryTable<Binary_GeometryNode>(bImport);
		bImport.close();
	}
	//DISABLE
	{
		for (auto i = 0; i < importTable3.header.totalEntries; ++i)
		{
			auto importObject = importTable3[i];
			auto importGeometry = importTable2[importObject->header.geometryID];
			auto importMesh = importTable[importGeometry->header.meshID];
			DLOG(37, "Reading from file:" << i );
			DLOG(33, "Mesh Stride:" << importMesh->header.vboStride << " AttributeCode: " << +importMesh->header.activeArrayAttributes << " Vertex Buffer Size: " << importMesh->header.payloadSize << " bytes Vertex Buffer Elements: " << importMesh->header.payloadSize / sizeof(float));

			auto nElements = (importMesh->header.eboOffset) / sizeof(float);
			auto nSubArrayElements = nElements / importMesh->header.vboStride;
			auto count{ 0 };
			for (auto subArray = 0; subArray < nSubArrayElements; ++subArray)
			{
				std::cout << "{";
				for (auto arrayIndex = 0; arrayIndex < importMesh->header.vboStride; ++arrayIndex)
					std::cout << importMesh->payload[subArray * importMesh->header.vboStride + arrayIndex] << " ,";
				std::cout << "}" << std::endl;
			}
			auto nIndices = (importMesh->header.payloadSize - importMesh->header.eboOffset) / sizeof(uint32_t);
			char* beginIndices = reinterpret_cast<char*>(importMesh->payload) + importMesh->header.eboOffset;
			std::cout << "{";
			for (auto index = 0; index < nIndices; ++index)
				std::cout << reinterpret_cast<uint32_t*>(beginIndices)[index] << ",";
			std::cout << "}" << std::endl;
		}
	}
	}
	return 0;
}