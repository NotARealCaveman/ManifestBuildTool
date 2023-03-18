#include "Binary_Types.h"

//Convert_MDB takes its resepective MDB_* datastructure and converts it into the binary format that will be stored as database records. these records and their respective tables are then exported to a .mdb(Manifest Database Binary) file which may be imported by the runtimem database program
using namespace Manifest_Persistence;

//GeometryObject
size_t Manifest_Persistence::Convert_MDB(const MDB_GeometryObject& geometryObject, Binary_GeometryObject& binaryGeometryObject)
{
	binaryGeometryObject.header.payloadSize = 0;
	binaryGeometryObject.header.geometryID = geometryObject.geometryID;
	binaryGeometryObject.header.meshID = geometryObject.meshID;
	binaryGeometryObject.header.morphID = geometryObject.morphID;
	DLOG(31, "Converting mdb_go with gID, mID, morphID : " << geometryObject.geometryID << " " << geometryObject.meshID << " " << geometryObject.morphID);
	return EntrySize(binaryGeometryObject);
}

//GeometryNode
size_t Manifest_Persistence::Convert_MDB(const MDB_GeometryNode& geometryNode, const ObjectRefBuildTable& objectRefBuildTable, const MaterialRefBuildTable& materialRefBuildTable, Binary_GeometryNode& binaryGeometryNode)
{		
	binaryGeometryNode.header.nodeID = geometryNode.nodeID;
	binaryGeometryNode.header.geometryID = *objectRefBuildTable.entries[geometryNode.objectRefID].geometryIDs;
	binaryGeometryNode.header.materialID = *materialRefBuildTable.entries[geometryNode.materialRefID].materialIDs;
	if (geometryNode.transform != BUFFER_NOT_PRESENT)
	{		
		//binaryGeometryNode.payload = new MFfloat[TransformSize];
		binaryGeometryNode.payload = New<MFfloat,ScratchPad< MFfloat>>(TransformSize);
		memcpy(binaryGeometryNode.payload, geometryNode.transform, binaryGeometryNode.header.payloadSize = sizeof(MFfloat) * TransformSize);
	}	
	DLOG(32, "Converting mdb_gn with nID, gID, mtlID: " << geometryNode.nodeID <<" " << binaryGeometryNode.header.geometryID << " " << binaryGeometryNode.header.materialID);
	return EntrySize(binaryGeometryNode);
};

//Material
size_t Manifest_Persistence::Convert_MDB(const MDB_Material& material, const TextureBuildTable& textureBuildTable, Binary_Material& binaryMaterial)
{	
	binaryMaterial.header.materialID = material.materialID;
	binaryMaterial.header.diffuseID = material.textureIDs[TextureTypes::DIFFUSE_TEXTURE];
	binaryMaterial.header.normalID = material.textureIDs[TextureTypes::NORMAL_TEXTURE];
	binaryMaterial.header.parallaxID = material.textureIDs[TextureTypes::PARALLAX_TEXTURE];	
	DLOG(33, "Converting mdb_mtl with mtlID, tdID, tnID, tpID: " << material.materialID << " " << binaryMaterial.header.diffuseID << " " << binaryMaterial.header.normalID << " "<< binaryMaterial.header.parallaxID);
	return EntrySize(binaryMaterial);
}

//Texture
size_t Manifest_Persistence::Convert_MDB(const MDB_Texture& texture, Binary_Texture& binaryTexture)
{
	//store base texture information
	auto& baseTexture = binaryTexture.header;
	baseTexture.textureID = texture.textureID;
	baseTexture.nChannels = GetCompositeBow(texture.textureInfo, TEXTURE_INFO_BOW_BITOFFSET);
	//set internal & data format information	
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
	binaryTexture.payload = New<Byte, ScratchPad<Byte>>(binaryTexture.header.payloadSize);
	for (auto element{ 0 }; element < binaryTexture.header.payloadSize; ++element)			
		binaryTexture.payload[element] = texture.channelData[element] * 255;	
	binaryTexture.header.payloadSize;
	DLOG(34, "Converting mdb_texture with tID: " << texture.textureID);
	return EntrySize(binaryTexture);
}

//Mesh
size_t Manifest_Persistence::Convert_MDB(const MDB_Mesh& mesh, const VertexBuildTables& vertexBuildTables, const IndexBuildTable& indexTable, Binary_Mesh& binaryMesh)
{
	binaryMesh.header.meshID = mesh.meshID;
	const auto& vertexTableIndices = mesh.vertexArrayIDs;
	///store vertex information
	//vertices
	const MDB_VertexArray* vertices = nullptr;
	ScratchPadVector<float> tempVertices;
	if (vertexTableIndices.vertexID != KEY_NOT_PRESENT)
	{
		vertices = &vertexBuildTables.vertexTable.entries[vertexTableIndices.vertexID];
		tempVertices.resize(vertices->elements);
		memcpy(tempVertices.data(), vertices->vertexData, sizeof(float) * vertices->elements);
		binaryMesh.header.payloadSize += vertices->elements;
		binaryMesh.header.vboStride += 3;
		binaryMesh.header.activeArrayAttributes |= (1 << 0);
	}
	//uvs
	const MDB_VertexArray* uvs = nullptr;
	ScratchPadVector<float> tempUVs;	
	if (vertexTableIndices.uvID != KEY_NOT_PRESENT)
	{
		uvs = &vertexBuildTables.uvTable.entries[vertexTableIndices.uvID];
		tempUVs.resize(uvs->elements);
		memcpy(tempUVs.data(), uvs->vertexData, sizeof(float) * uvs->elements);
		binaryMesh.header.payloadSize += uvs->elements;
		binaryMesh.header.vboStride += 2;
		binaryMesh.header.activeArrayAttributes |= (1 << 1);
	}
	//normals
	const MDB_VertexArray* normals = nullptr;
	ScratchPadVector<float> tempNormals;		
	if (vertexTableIndices.normalID != KEY_NOT_PRESENT)
	{
		normals = &vertexBuildTables.normalTable.entries[vertexTableIndices.normalID];
		tempNormals.resize(normals->elements);
		memcpy(tempNormals.data(), normals->vertexData, sizeof(float) * normals->elements);
		binaryMesh.header.payloadSize += normals->elements;
		binaryMesh.header.vboStride += 3;
		binaryMesh.header.activeArrayAttributes |= (1 << 2);
	}
	//tangents
	const MDB_VertexArray* tangents = nullptr;
	ScratchPadVector<float> tempTangents;
	if (vertexTableIndices.tangentID != KEY_NOT_PRESENT)
	{
		tangents = &vertexBuildTables.tangentTable.entries[vertexTableIndices.tangentID];
		tempTangents.resize(tangents->elements);
		memcpy(tempTangents.data(), tangents->vertexData, sizeof(float) * tangents->elements);
		binaryMesh.header.payloadSize += tangents->elements;
		binaryMesh.header.vboStride += 3;
		binaryMesh.header.activeArrayAttributes |= (1 << 3);
	}
	//bitangents	
	const MDB_VertexArray* bitangents = nullptr;
	ScratchPadVector<float> tempBitangents;
	if (vertexTableIndices.bitangentID != KEY_NOT_PRESENT)
	{
		bitangents = &vertexBuildTables.bitangentTable.entries[vertexTableIndices.bitangentID];
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
	//binaryMesh.payload = new float[binaryMesh.header.payloadSize];//reserves enough memory for each float	
	binaryMesh.payload = New<MFfloat, ScratchPad<MFfloat>>(binaryMesh.header.payloadSize);
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

	DLOG(35, "Converting mdb_mesh with mID: " << mesh.meshID);
	return EntrySize(binaryMesh);
}

//Terrain
size_t Manifest_Persistence::Convert_MDB(const MDB_Terrain& terrain, Binary_Terrain& binaryTerrain)
{
	auto& header{ binaryTerrain.header };
	header.payloadSize = 0;
	header.terrainHash = terrain.terrainIndexHash;	

	DLOG(36, "Converting mdb_terrain terrainID: " << terrain.terrainID);
	return EntrySize(binaryTerrain);
}

//VoxelMap
size_t Manifest_Persistence::Convert_MDB(const MDB_VoxelMap& voxelMap, Binary_VoxelMap& binaryVoxelMap)
{
	auto& header{ binaryVoxelMap.header };
	header.worldLOD = voxelMap.worldLOD;
	header.xChunks = voxelMap.xChunks;
	header.zChunks = voxelMap.zChunks;
	header.payloadSize =
		(header.nVoxels = voxelMap.nVoxels) *
		(header.mVoxels = voxelMap.mVoxels) *
		(header.hVoxels = voxelMap.hVoxels);
	binaryVoxelMap.payload = New<MFint8, ScratchPad<MFint8>>(header.payloadSize);
	memcpy(binaryVoxelMap.payload, voxelMap.mapSDF, header.payloadSize);

	DLOG(36, "Converting mdb_voxelmap mapID: " << voxelMap.mapID);
	return EntrySize(binaryVoxelMap);
}