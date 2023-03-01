#pragma once
#include <string>

#include <ManifestParser/MDD/MDD_Terrain.h>

namespace Manifest_Parser
{	
	void ExportTerrain(const std::string& terrainFile, const MFu32& xBlocks, const MFu32& zBlocks, const MFu32& yBlocks, const MFu8& lod, const MFint8* field);
}