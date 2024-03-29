#pragma once
#include <thread>

#include <ManifestUtility/DebugLogger.h>

#include <ManifestGLFramework/ManifestMath/Core.h>
#include <ManifestGLFramework/ManifestMath/Point3.h>
#include <ManifestGLFramework/ManifestMath/PerlinNoise.hpp>
#include <ManifestGLFramework/ManifestMath/Integer3D.h>

using namespace Manifest_Math;

namespace Manifest_Terrain
{
	using Voxel = MFint8;

	struct VoxelMap
	{		
		MFu32 nVoxels;
		MFu32 mVoxels;	
		MFu32 hVoxels;
		MFu8 lod;			

		Voxel* field;
	};

	//generates a voxel map large enough for the requested number of blocks per axis and a max lod level
	VoxelMap GenerateVoxelMap(const MFu32& seed, const MFu8& lod, const MFu32& nBlocks, const MFu32& mBlocks, const MFu32& hBlocks, const int& DF);
		
	static constexpr MFint32 groundLevel = 2;	
	static constexpr MFint32 ceilingLevel = groundLevel + 10;
	//DENSITY FUNCTIONS	
	//flat plane
	static inline Voxel DensityFunction_0(siv::PerlinNoise& noise, const MFu32 i, const MFu32& j, const MFu32& k);	
	//vertical plane
	static inline Voxel DensityFunction_1(siv::PerlinNoise& noise, const MFu32 i, const MFu32& j, const MFu32& k);
	//terrain func 1
	static inline Voxel DensityFunction_2(siv::PerlinNoise& noise, const MFu32 i, const MFu32& j, const MFu32& k);
	//terrain func 2
	static inline Voxel DensityFunction_3(siv::PerlinNoise& noise, const MFu32 i, const MFu32& j, const MFu32& k);
}