#pragma once
#include <ManifestUtility/GLDefinitions.h>

#include "Binary_Table.h"

namespace Manifest_Persistence
{
	typedef BinaryTable<Binary_Mesh> BinaryMeshTable;
	typedef BinaryTable<Binary_Texture> BinaryTextureTable;
	typedef BinaryTable<Binary_Material> BinaryMaterialTable;
	typedef BinaryTable<Binary_GeometryObject> BinaryGeometryObjectTable;
	typedef BinaryTable<Binary_GeometryNode> BinaryGeometryNodeTable;	
}