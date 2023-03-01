#include "ManifestDataDescriptor.h"

using namespace Manifest_Parser;


void Manifest_Parser::ExportTerrain(const std::string& terrainFile, const MFu32& xBlocks, const MFu32& zBlocks, const MFu32& yBlocks, const MFu8& lod, const MFint8* field)
{
	const auto nVoxels{ (19 + (xBlocks - 1) * 17) << lod };
	const auto mVoxels{ (19 + (zBlocks - 1) * 17) << lod };
	const auto hVoxels{ (19 + (yBlocks - 1) * 17) << lod };
	const auto voxelCount{ nVoxels + mVoxels + hVoxels };
	std::ofstream mddExport{ terrainFile, std::ios::out };
	if (mddExport.is_open())
	{
		//terrain strcture header
		mddExport.write("Terrain\n{\n", sizeof(char[11-1]));
		//terrain xblock substructure
		mddExport.write("\tu32 xBlocks {", sizeof(char[15-1]));
		mddExport.write(std::to_string(xBlocks).data(), sizeof(char) * std::to_string(xBlocks).size());
		mddExport.write("}\n", sizeof(char[3-1]));
		//terrain zblock substructure
		mddExport.write("\tu32 zBlocks {", sizeof(char[15-1]));
		mddExport.write(std::to_string(zBlocks).data(), sizeof(char) * std::to_string(zBlocks).size());
		mddExport.write("}\n", sizeof(char[3-1]));
		//terrain yblock substructure
		mddExport.write("\tu32 yBlocks {", sizeof(char[15-1]));
		mddExport.write(std::to_string(yBlocks).data(), sizeof(char) * std::to_string(yBlocks).size());
		mddExport.write("}\n", sizeof(char[3-1]));
		//terrain payload
		mddExport.write("\ti8 field[", sizeof(char[11 - 1]));
		mddExport.write(std::to_string(voxelCount).data(), sizeof(char) * std::to_string(voxelCount).size());
		mddExport.write("]{", sizeof(char[3 - 1]));
		for (auto i{ 0 }; i < nVoxels; ++i)
		{
			for (auto j{ 0 }; j < mVoxels; ++j)
			{
				for (auto k{ 0 }; k < hVoxels; ++k)
				{
					const auto& sample = field[i + nVoxels * (j + mVoxels * k)];
					mddExport.write(std::to_string(sample).data(), sizeof(char) * std::to_string(sample).size());
					mddExport.write(", ", sizeof(char[3 - 1]));
				}
			}
		}
		mddExport.write("}\n}", sizeof(char[4 - 1]));
		mddExport.close();
	}
}