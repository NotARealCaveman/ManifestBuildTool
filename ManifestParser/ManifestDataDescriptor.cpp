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
				//write world metrics 
				//lod
				std::string exportString{ "Metric $WORLD_LOD(key = \"lod\") {u8 { " + std::to_string(lod) + " }}\n" };
				mddExport.write(exportString.data(), sizeof(char) * exportString.size());
				//x
				exportString = "Metric $X_CHUNK_INDEX(key = \"xChunkIndex\") {u32 { " + std::to_string(x) + " }}\n";
				mddExport.write(exportString.data(), sizeof(char) * exportString.size());
				//z
				exportString = "Metric $Z_CHUNK_INDEX(key = \"zChunkIndex\") {u32 { " + std::to_string(z) + " }}\n\n";
				mddExport.write(exportString.data(), sizeof(char) * exportString.size());	
				//terrain header
				exportString = "Terrain $Chunk" + std::to_string(index) + "\n{\n";
				mddExport.write(exportString.data(), sizeof(char) * exportString.size());				
				//chunk index
				exportString = "\tu8 %index { " + std::to_string(index) + " }\n";
				mddExport.write(exportString.data(), sizeof(char) * exportString.size());
				//chunk sdf
				exportString = "\ti8 %field [" + std::to_string(voxelCount) + "]{\n\t\t{";
				mddExport.write(exportString.data(), sizeof(char) * exportString.size());
				for (auto i{ 0 }; i < nVoxels; ++i)
				{
					for (auto j{ 0 }; j < mVoxels; ++j)
					{
						for (auto k{ 0 }; k < hVoxels; ++k)
						{		
							//Integer3D offset{x * BASE_CELLS_PER_BLOCK,z * BASE_CELLS_PER_BLOCK,0};
						//	offset = offset << lod;
							//const auto& sample = field[(i+offset.i) + nVoxels * ((j+offset.j) + mVoxels * (k+offset.k))];
							//DLOG(35, +sample <<" index: " << i + nVoxels * (j + mVoxels * k));
							exportString = std::to_string(field[i + nVoxels * (j + mVoxels * k)]) + ",";
							mddExport.write(exportString.data(), sizeof(char) * exportString.size());
							
						}
					}
				}//remove excess "," from last entry
				mddExport.seekp(mddExport.tellp() - std::streampos{ 1 });
				exportString = "}\n\t}\n}";
				mddExport.write(exportString.data(), sizeof(char) * exportString.size());
			}
			mddExport.close();
		}
	/*
mddExport.write("Terrain\n{\n", sizeof(char[11-1]));
mddExport.write(std::to_string(xChunks).data(), sizeof(char) * std::to_string(xChunks).size());
//terrain xblock substructure
mddExport.write("\tu32 xBlocks {", sizeof(char[15-1]));
mddExport.write(std::to_string(xChunks).data(), sizeof(char) * std::to_string(xChunks).size());
mddExport.write("}\n", sizeof(char[3-1]));
//terrain zblock substructure
mddExport.write("\tu32 zBlocks {", sizeof(char[15-1]));
mddExport.write(std::to_string(zChunks).data(), sizeof(char) * std::to_string(zChunks).size());
mddExport.write("}\n", sizeof(char[3-1]));
//terrain yblock substructure
mddExport.write("\tu32 yBlocks {", sizeof(char[15-1]));
mddExport.write(std::to_string(yChunks).data(), sizeof(char) * std::to_string(yChunks).size());
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
*/
}