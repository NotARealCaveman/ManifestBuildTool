#include "Binary_Mesh.h"

using namespace Manifset_Persistence;

/*
BinaryMeshTable Manifset_Persistence::BuildBinaryTable(const MeshTable& meshTable, const VertexTables& vertexTables, const IndexTable& indexTable)
{
	BinaryMeshTable result;		
	result.entries = new Binary_Mesh[result.header.totalEntries = meshTable.entries.size()];
	for(auto entry = 0; entry < result.header.totalEntries; ++entry)
		result.header.dynamicTableSize += Convert_MDB(meshTable.entries[entry], vertexTables, indexTable, result.entries[entry]);
	return result;
}	
*/

size_t Manifset_Persistence::Convert_MDB(const MDB_Mesh& mesh, const VertexTables& vertexTables, const IndexTable& indexTable, Binary_Mesh& binaryMesh)
{	
	const auto& vertexTableIndices = mesh.vertexArrayIDs;	
	size_t nElements{ 0 };
		
	//vertices
	const MDB_VertexArray* vertices = nullptr;	
	std::vector<float> tempVertices;
	if (vertexTableIndices.vertexID != KEY_NOT_PRESENT)
	{
		vertices = &vertexTables.vertexTable.entries[vertexTableIndices.vertexID];
		tempVertices.resize(vertices->elements);
		memcpy(tempVertices.data(), vertices->vertexData, sizeof(float) * vertices->elements);		
		binaryMesh.header.payloadSize += vertices->elements;
		binaryMesh.header.vboStride += 3;
		binaryMesh.header.activeArrayAttributes |= (1<<0);
	}
	//uvs
	const MDB_VertexArray* uvs = nullptr;
	std::vector<float> tempUVs;
	if (vertexTableIndices.uvID != KEY_NOT_PRESENT)
	{
		uvs = &vertexTables.uvTable.entries[vertexTableIndices.uvID];
		tempUVs.resize(uvs->elements);
		memcpy(tempUVs.data(), uvs->vertexData, sizeof(float) * uvs->elements);
		binaryMesh.header.payloadSize += uvs->elements;
		binaryMesh.header.vboStride += 2;
		binaryMesh.header.activeArrayAttributes |= (1 << 1);
	}
	//normals
	const MDB_VertexArray* normals = nullptr;
	std::vector<float> tempNormals;
	if (vertexTableIndices.normalID != KEY_NOT_PRESENT)
	{
		normals = &vertexTables.normalTable.entries[vertexTableIndices.normalID];
		tempNormals.resize(normals->elements);
		memcpy(tempNormals.data(), normals->vertexData, sizeof(float) * normals->elements);
		binaryMesh.header.payloadSize += normals->elements;
		binaryMesh.header.vboStride += 3;
		binaryMesh.header.activeArrayAttributes |= (1 << 2);
	}
	//tangents
	const MDB_VertexArray* tangents = nullptr;
	std::vector<float> tempTangents;
	if (vertexTableIndices.tangentID != KEY_NOT_PRESENT)
	{
		tangents = &vertexTables.tangentTable.entries[vertexTableIndices.tangentID];
		tempTangents.resize(tangents->elements);
		memcpy(tempTangents.data(), tangents->vertexData, sizeof(float) * tangents->elements);
		binaryMesh.header.payloadSize += tangents->elements;
		binaryMesh.header.vboStride += 3;
		binaryMesh.header.activeArrayAttributes |= (1 << 3);
	}
	//bitangents	
	const MDB_VertexArray* bitangents = nullptr;
	std::vector<float> tempBitangents;
	if (vertexTableIndices.bitangentID != KEY_NOT_PRESENT)
	{
		bitangents = &vertexTables.bitangentTable.entries[vertexTableIndices.bitangentID];
		tempBitangents.resize(bitangents->elements);
		memcpy(tempBitangents.data(), bitangents->vertexData, sizeof(float) * bitangents->elements);
		binaryMesh.header.payloadSize += bitangents->elements;
		binaryMesh.header.vboStride += 3;
		binaryMesh.header.activeArrayAttributes |= (1 << 4);
	}
	binaryMesh.payload = new float[binaryMesh.header.payloadSize];//reserves enough memory for each float
	binaryMesh.header.payloadSize *= sizeof(float);//store total number of bytes of data
	//interleave buffer data
	for (auto bufferIndex = 0; bufferIndex < vertices->elements/3; ++bufferIndex)
	{
		auto baseOffset{ bufferIndex * binaryMesh.header.vboStride };
		auto attributeOffset{ 0 };
		if (vertices)
		{
			memcpy(&binaryMesh.payload[baseOffset + attributeOffset], &tempVertices[bufferIndex * 3], sizeof(float) * 3);
			attributeOffset += 3;
		}
		if (uvs)
		{
			memcpy(&binaryMesh.payload[baseOffset + attributeOffset], &tempUVs[bufferIndex * 2], sizeof(float) * 2);
			attributeOffset += 2;
		}
		if (normals)
		{
			memcpy(&binaryMesh.payload[baseOffset + attributeOffset], &tempNormals[bufferIndex * 3], sizeof(float) * 3);
			attributeOffset += 3;
		}
		if (tangents)
		{
			memcpy(&binaryMesh.payload[baseOffset + attributeOffset], &tempTangents[bufferIndex * 3], sizeof(float) * 3);
			attributeOffset += 3;
		}
		if (bitangents)		
			memcpy(&binaryMesh.payload[baseOffset + attributeOffset], &tempBitangents[bufferIndex * 3], sizeof(float) * 3);
	}
	
	return EntrySize(binaryMesh);
}