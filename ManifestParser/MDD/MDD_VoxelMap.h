#pragma once
#include <map>
#include <string>

#include <ManifestParser/OpenDDL/DDL_Primitives.h>
#include <ManifestParser/OpenDDL/DDL_Buffers.h>

namespace Manifest_Parser
{
	struct MDD_VoxelMap : public RegisteredBuilder
	{
		struct PropertyList
		{
			static constexpr uint32_t N_VOXELS{ 0 };
			static constexpr uint32_t M_VOXELS{ N_VOXELS + 1 };
			static constexpr uint32_t H_VOXELS{ M_VOXELS + 1 };
			static const std::map<std::string, DDL_BufferType> typeProperties;
		};
		DDL_Structure* Build(const std::string_view& partitionedStructureView, DDL_ReferenceMap& referenceMap)final;//tbd
		//substructures
		DDL_Int8 field;

		//properties
		MFu32 nVoxels{ UINT32_MAX };
		MFu32 mVoxels{ UINT32_MAX };
		MFu32 hVoxels{ UINT32_MAX };
	};
}