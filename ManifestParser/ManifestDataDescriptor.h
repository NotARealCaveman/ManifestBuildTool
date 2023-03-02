#pragma once
#include <string>

#include <ManifestMathCOPY/Integer3D.h>
#include <ManifestParser/MDD/MDD_Terrain.h>

using namespace Manifest_Math;

namespace Manifest_Parser
{	
	constexpr MFint32 BASE_CELLS_PER_BLOCK{ 16 };

	void ExportTerrainMDD(const std::string& terrainFile, const MFu32& xBlocks, const MFu32& zBlocks, const MFu32& yBlocks, const MFu8& lod, const MFint8* field);
}