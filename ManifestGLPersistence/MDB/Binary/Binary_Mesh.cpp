#include "Binary_Mesh.h"

using namespace Manifset_Persistence;

size_t Manifset_Persistence::Convert_MDB(const MDB_Mesh& mesh, const VertexTables& vertexTables, const IndexTable& indexTable, Binary_Mesh& binaryMesh)
{	
	const auto& vertexTableIndices = mesh.vertexArrayIDs;			
	///store vertex information
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
	///store index information
	const MDB_IndexArray& indices = indexTable.entries[mesh.indexArrayID];
	auto nIndices = indices.elements;
	binaryMesh.header.payloadSize += nIndices;
	binaryMesh.payload = new float[binaryMesh.header.payloadSize];//reserves enough memory for each float	
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
	//store index information
	auto indexOffset = (binaryMesh.header.payloadSize - nIndices);
	memcpy(&binaryMesh.payload[indexOffset], indices.indexData, sizeof(uint32_t) * nIndices);
	binaryMesh.header.eboOffset = indexOffset * sizeof(float);
	//convert to total bytes
	binaryMesh.header.payloadSize *= sizeof(float);
	
	
	return EntrySize(binaryMesh);
}