#include "Voxel.h"

using namespace Manifest_Terrain;

void ParallelMapGeneration(siv::PerlinNoise& noise, Voxel* field, int i, int j, int k, int XZStride, int cap, const int df)
{		
	for (k=0 ; k < cap; ++k)
		for (j=0;  j < XZStride; ++j)
			for (i=0; i < XZStride; ++i)
			{
				const auto index{ (i) +XZStride * ((j)+XZStride * (k )) };
				//LOG(32, "Index: " << index);
				switch(df)
				{
					case 0:
						field[index] = DensityFunction_0(noise, i, j, k);
						break;
					case 1:
						field[index] = DensityFunction_1(noise, i, j, k);
						break;
					case 3:
						field[index] = DensityFunction_3(noise, i, j, k);
						break;
				}
			}	
}


VoxelMap Manifest_Terrain::GenerateVoxelMap(const MFu32& seed, const MFu8& lod, const MFu32& nBlocks, const MFu32& mBlocks, const MFu32& hBlocks, const int& DF)
{
	auto noise = siv::PerlinNoise{ seed };
	const MFu32 nVoxels = (19 + (nBlocks - 1) * 17) << lod;
	const MFu32 mVoxels = (19 + (mBlocks - 1) * 17) << lod;
	const MFu32 hVoxels = (19 + (hBlocks - 1) * 17) << lod;
	VoxelMap result{ nVoxels,mVoxels,hVoxels,lod };
	result.field = new Voxel[result.nVoxels * result.mVoxels * result.hVoxels];

	auto nThreads{ 7 };
	auto cap{ hVoxels / nThreads };
	std::vector<std::thread> threads;
	for (int thread{ 0 }; thread < nThreads ; ++thread)
	{
		const auto k{ ((thread+1) * cap) };
		threads.emplace_back(std::thread{ ParallelMapGeneration,std::ref(noise),result.field,0,0,0,mVoxels,k,DF });
	}

	for (auto& thread : threads)
		thread.join();
	/*
	for (MFu32 k = 0; k < result.hVoxels; ++k)
		for (MFu32 j = 0; j < result.mVoxels; ++j)
			for (MFu32 i = 0; i < result.nVoxels; ++i)
			{
				switch (DF)
				{
				case 0:
					result.field[i + result.nVoxels * (j + result.mVoxels * k)] = DensityFunction_0(noise, i, j, k);
					break;
				case 1:
					result.field[i + result.nVoxels * (j + result.mVoxels * k)] = DensityFunction_1(noise, i, j, k);
					break;
				case 2:
					result.field[i + result.nVoxels * (j + result.mVoxels * k)] = DensityFunction_2(noise, i, j, k);
					break;
				case 3:
					result.field[i + result.nVoxels * (j + result.mVoxels * k)] = DensityFunction_3(noise, i, j, k);
					break;
				}				
			}
			*/
	return result;
}

Voxel Manifest_Terrain::DensityFunction_0(siv::PerlinNoise& noise, const MFu32 i, const MFu32& j, const MFu32& k)
{
	if (k < groundLevel)
		return -1;
	else if (k == groundLevel)
		return 0;
	else
		return 1;

}


Voxel Manifest_Terrain::DensityFunction_1(siv::PerlinNoise& noise, const MFu32 i, const MFu32& j, const MFu32& k)
{	
	if (j < groundLevel)
		return -1;
	else if (j == groundLevel)
		return 0;
	else
		return 1;
}

Voxel Manifest_Terrain::DensityFunction_2(siv::PerlinNoise& noise, const MFu32 i, const MFu32& j, const MFu32& k)
{	
	if (k < groundLevel)
		return -1;	
	if (k > ceilingLevel)
		return 1;	
	float _i = 0.5 * i;
	float _j = 0.5 * j;
	auto sample = 64 * noise.noise2D(_i * 0.0025, 0.5 * _j * 0.0025);
	sample += 32 * noise.octave2D_11(_i * 0.0325, _j * 0.0325, 2, .5);
	sample += 16 * noise.octave2D_11(_i * 0.0525, _j * 0.0525, 4, .75);
	sample += 32 * noise.octave2D_11(_i * 0.0625, _j * 0.0625, 8, .15);		
	sample += 127 * noise.noise3D(_i * 0.0025, _j * 0.0025, k * 0.0025);
	sample += 64 * noise.octave3D(_i * 0.0125, _j * 0.0125, k * 0.02015, 2, 0.5);
	
	sample = std::min(std::max((int)sample, -127), 127);
	return  sample;
};

Voxel Manifest_Terrain::DensityFunction_3(siv::PerlinNoise& noise, const MFu32 i, const MFu32& j, const MFu32& k)
{
	if (k > ceilingLevel)
		return 1;
	if (k < groundLevel)
		return -1;
	if (k == groundLevel)
		return 0;
	MFfloat sample = 0;
	sample += noise.noise3D(i * 0.009, j * 0.009,k*0.009);
	sample += noise.noise2D(i * 0.009, j * 0.009);
	if (k > 20)
	{
		sample += 0.75*noise.octave3D(i * 0.01, j * 0.01, k * 0.01, 1, .15);
		sample -= 0.75 * noise.octave2D(i * 0.01, j * 0.01, 1, .15);
		if (k > 65)
		{
			sample -= 0.5*noise.octave3D_01(i * 0.01, j * 0.01, k * 0.01, 2, .25);
			sample += 0.5 * noise.octave2D_01(i * 0.01, j * 0.01, 2, .25);
			if (k > 100)
			{
				sample += 0.25*noise.octave3D(i * 0.02, j * 0.02, k * 0.02, 3, .35);
				sample -= 0.25 * noise.octave3D(i * 0.02, j * 0.02, 3, .35);
			}
		}
	}

	sample = std::fminf(std::fmaxf(sample, -1), 1);
	return  sample * 127;
}