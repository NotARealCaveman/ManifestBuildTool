#include "ManifestDataDescriptor.h"

using namespace Manifest_Parser;


void Manifest_Parser::ExportTerrainMDD(const std::string& terrainFile, const MFu32& xChunks, const MFu32& zChunks, const MFu32& yChunks, const MFu8& lod, const MFint8* field)
{
	const auto nVoxels{ 19 << lod };
	const auto mVoxels{ 19 << lod };
	const auto hVoxels{ 19 << lod };
	const auto voxelCount{ nVoxels + mVoxels + hVoxels };
	//terrain structure headers
	for (int z{ 0 }; z < zChunks; ++z)
		for (int x{ 0 }; x < xChunks; ++x)
		{
			auto index = z * xChunks + x;
			auto fileName = terrainFile;
			fileName.insert(fileName.find_first_of('.'), std::to_string(index));
			std::ofstream mddExport{ fileName, std::ios::out};
			if (mddExport.is_open())
			{
				//terrain header
				auto exportString = "Terrain $Chunk" + std::to_string(index) + "\n{\n";
				mddExport.write(exportString.data(), sizeof(char) * exportString.size());				
				//chunk index
				exportString = "\tu8 %index { " + std::to_string(index) + " }\n}";
				mddExport.write(exportString.data(), sizeof(char) * exportString.size());	
			}
			mddExport.close();
		}
}

void Manifest_Parser::ExportVoxelMapMDD(const std::string& voxelMapFile, const MFu32& xChunks, const MFu32& zChunks, const MFu32& yChunks, const MFu8& lod, const MFint8* field)
{
	const auto nVoxels{ (19 + (xChunks - 1) * 17) << lod };
	const auto mVoxels{ (19 + (zChunks - 1) * 17) << lod };
	const auto hVoxels{ (19 + (yChunks - 1) * 17) << lod };
	const auto voxelCount{ nVoxels + mVoxels + hVoxels };

	std::ofstream mddExport{ voxelMapFile, std::ios::out };
	if (mddExport.is_open())
	{
		//write world metrics 
		//lod
		std::string exportString{ "Metric $WORLD_LOD (key = \"lod\") {u8 { " + std::to_string(lod) + " }}\n" };
		mddExport.write(exportString.data(), sizeof(char) * exportString.size());
		//x
		exportString = "Metric $WORLD_X_CHUNKS (key = \"xChunks\") {u32 {" + std::to_string(xChunks) + "}}\n";
		mddExport.write(exportString.data(), sizeof(char) * exportString.size());
		//z
		exportString = "Metric $WORLD_Z_CHUNKS (key = \"zChunks\") {u32 {" + std::to_string(zChunks) + "}}\n\n";
		mddExport.write(exportString.data(), sizeof(char) * exportString.size());
		//voxel map header
		exportString = "VoxelMap (nVoxels = "+std::to_string(nVoxels)+", mVoxels = "+ std::to_string(mVoxels) +", hVoxels = " + std::to_string(hVoxels) + ")" + "\n{\n";
		mddExport.write(exportString.data(), sizeof(char) * exportString.size());
		//map sdf
		exportString = "\ti8 %field [" + std::to_string(voxelCount) + "]{\n\t\t{";
		mddExport.write(exportString.data(), sizeof(char) * exportString.size());
		for (auto sample{ 0 }; sample < voxelCount; ++sample)
		{
			exportString = std::to_string(field[sample]) + ",";
			mddExport.write(exportString.data(), sizeof(char) * exportString.size());
		}//remove excess "," from last entry
		mddExport.seekp(mddExport.tellp() - std::streampos{ 1 });
		exportString = "}\n\t}\n}";
		mddExport.write(exportString.data(), sizeof(char) * exportString.size());
	}
	mddExport.close();
}