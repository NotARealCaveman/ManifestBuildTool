#pragma once
#include <vector>	

#include "Binary_Database.h"

namespace Manifest_Persistence
{

	template<typename Key, typename Value>
	struct Table
	{
		MFsize tableEntries;
		Key* keys;
		Value* values;
	};
	//pairs database IDs to generated runtime IDs
	typedef Table<PrimaryKey, UniqueKey> RuntimeIDTable;

	struct GeometryNodes
	{
		RuntimeIDTable nodeInstances;
	};

	struct GeometryObjects
	{
		RuntimeIDTable objectInstances;
	};

	struct Meshes
	{
		RuntimeIDTable meshInstances;
	};

	struct Manifest_Material
	{
		PrimaryKey materialTexture
	};

	struct Materials
	{
		RuntimeIDTable materialInstances;
	};

	struct Manifest_Texture
	{		
		PrimaryKey manifestTextureID;
		UniqueKey textureResourceID;
	};

	struct Textures
	{
		RuntimeIDTable textureInstances;
		PrimaryKey* manifestTextureIDs;
		UniqueKey* textureResourceIDs;
	};

	class Runtime_Database
	{
		public:
			GeometryNodes geometryNodes;
			GeometryObjects geometryObjects;
			Meshes meshes;
			Materials materials;
			Textures textures;
	};
}