#pragma once
#include "Binary_Table.h"

namespace Manifest_Persistence
{
	typedef BinaryTable<Binary_Mesh> BinaryMeshTable;
	typedef BinaryTable<Binary_GeometryObject> BinaryGeometryObjectTable;
	typedef BinaryTable<Binary_GeometryNode> BinaryGeometryNodeTable;
}