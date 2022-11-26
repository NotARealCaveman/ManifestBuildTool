#include "Binary_Types.h"

//Convert_MDB takes its resepective MDB_* datastructure and converts it into the binary format that will be stored as database records. these records and their respective tables are then exported to a .mdb(Manifest Database Binary) file which may be imported by the runtimem database program
using namespace Manifest_Persistence;

//GeometryObject
size_t Manifest_Persistence::Convert_MDB(const MDB_GeometryObject& geometryObject, Binary_GeometryObject& binaryGeometryObject)
{
	binaryGeometryObject.header.payloadSize = 0;
	binaryGeometryObject.header.meshID = geometryObject.meshID;
	binaryGeometryObject.header.morphID = geometryObject.morphID;
	return EntrySize(binaryGeometryObject);
}

//GeometryNode
size_t Manifest_Persistence::Convert_MDB(const MDB_GeometryNode& geometryNode, const ObjectRefTable& objectRefTable, const MaterialRefTable& materialRefTable, Binary_GeometryNode& binaryGeometryNode)
{		
	binaryGeometryNode.header.geometryID = *objectRefTable.entries[geometryNode.objectRefID].geometryIDs;
	binaryGeometryNode.header.materialID = *materialRefTable.entries[geometryNode.materialRefID].materialIDs;	
	if (geometryNode.transform != BUFFER_NOT_PRESENT)
	{
		binaryGeometryNode.header.payloadSize =  TransformSize;
		binaryGeometryNode.payload = new float[TransformSize];
		memcpy(binaryGeometryNode.payload, geometryNode.transform, sizeof(float) * TransformSize);
	}
	return EntrySize(binaryGeometryNode);
};

//Material
size_t Manifest_Persistence::Convert_MDB(const MDB_Material& material, const TextureTable& textureTable, Binary_Material& binaryMaterial)
{	
	for (const auto& texture : textureTable.entries)
		if (texture.materialID == material.materialID)
		{
			switch (texture.textureType)
			{
				case TextureTypes::DIFFUSE_TEXTURE:
					binaryMaterial.header.diffuseID = texture.textureID;
					break;
				case TextureTypes::NORMAL_TEXTURE:
					binaryMaterial.header.noramlID = texture.textureID;
					break;
				case TextureTypes::PARALLAX_TEXTURE:
					binaryMaterial.header.parallaxID= texture.textureID;
					break;
				DEFAULT_BREAK
			}
		}

	return EntrySize(binaryMaterial);
}

//Texture
size_t Manifest_Persistence::Convert_MDB(const MDB_Texture& texture, Binary_Texture& binaryTexture)
{
	//store base texture information
	Manifest_Texture& baseTexture = binaryTexture.header;
	baseTexture.nChannels = GetCompositeBow(texture.textureInfo, TEXTURE_INFO_BOW_BITOFFSET);
	//set internal and data format information	
	switch (baseTexture.nChannels)
	{
		case ChannelTypes::R:
			baseTexture.internalFormat = GL_RED;
			baseTexture.dataFormat = GL_R;
			break;
		case ChannelTypes::RGB:
			baseTexture.internalFormat = texture.textureType == TextureTypes::DIFFUSE_TEXTURE ?	GL_SRGB8 : GL_RGB8;
			baseTexture.dataFormat = GL_RGB;
			break;
		case ChannelTypes::RGBA:
			baseTexture.internalFormat = texture.textureType == TextureTypes::DIFFUSE_TEXTURE ?	GL_SRGB8_ALPHA8 : GL_RGBA8;
			baseTexture.dataFormat = GL_RGBA;
			break;
		DEFAULT_BREAK
	}
	
	baseTexture.width = GetCompositeWard(texture.textureDimensions, TEXTURE_DIMENSION_BOW_BITOFFSET);
	baseTexture.height = GetCompositeBow(texture.textureDimensions, TEXTURE_DIMENSION_BOW_BITOFFSET);
	//store texture payload information
	binaryTexture.header.payloadSize = GetCompositeWard(texture.textureInfo, TEXTURE_INFO_BOW_BITOFFSET);
	binaryTexture.payload = new float[binaryTexture.header.payloadSize];	
	memcpy(binaryTexture.payload, texture.channelData, (binaryTexture.header.payloadSize*=sizeof(float)));

	return EntrySize(binaryTexture);
}

//Mesh
size_t Manifest_Persistence::Convert_MDB(const MDB_Mesh& mesh, const VertexTables& vertexTables, const IndexTable& indexTable, Binary_Mesh& binaryMesh)
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
		binaryMesh.header.activeArrayAttributes |= (1 << 0);
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
	for (auto bufferIndex = 0; bufferIndex < vertices->elements / 3; ++bufferIndex)
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