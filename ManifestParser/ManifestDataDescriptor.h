#pragma once
#include <string>

#include <ManifestMathCOPY/Integer3D.h>
#include <ManifestParser/MDD/MDD_Terrain.h>
#include <ManifestParser/MDD/MDD_VoxelMap.h>
#include <ManifestParser/MDD/MDD_PhysicsNode.h>
#include <ManifestParser/MDD/MDD_GameObject.h>

using namespace Manifest_Math;

namespace Manifest_Parser
{	
	constexpr MFint32 BASE_CELLS_PER_BLOCK{ 16 };

	void ExportTerrainMDD(const std::string& terrainFile, const MFu32& xChunks, const MFu32& zChunks, const MFu32& yChunks, const MFu8& lod, const MFint8* field);

	void ExportVoxelMapMDD(const std::string& voxelMapFile, const MFu32& xChunks, const MFu32& zChunks, [[maybe_unused]] const MFu32& yChunks, const MFu8& lod, const MFint8* field);
}