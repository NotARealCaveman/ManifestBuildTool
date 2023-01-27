#pragma once

#include <ManifestParser/DDL_ExtendedTypes.h>

namespace Manifest_Parser
{
	struct GEX_BufferTypes
	{
		static constexpr DDL_BufferType GEX_Animation{ DDL_ExtendedTypes::NEXT_BUFFER_ID };
		static constexpr DDL_BufferType GEX_Atten{ GEX_Animation + 1 };
		static constexpr DDL_BufferType GEX_BoneCountArray{ GEX_Atten + 1 };
		static constexpr DDL_BufferType GEX_BoneIndexArray{ GEX_BoneCountArray + 1 };
		static constexpr DDL_BufferType GEX_BoneNode{ GEX_BoneIndexArray + 1 };
		static constexpr DDL_BufferType GEX_BoneRefArray{ GEX_BoneNode + 1 };
		static constexpr DDL_BufferType GEX_BoneWeightArray{ GEX_BoneRefArray + 1 };
		static constexpr DDL_BufferType GEX_CameraNode{ GEX_BoneWeightArray + 1 };
		static constexpr DDL_BufferType GEX_CameraObject{ GEX_CameraNode + 1 };
		static constexpr DDL_BufferType GEX_Clip{ GEX_CameraObject + 1 };
		static constexpr DDL_BufferType GEX_Color{ GEX_Clip + 1 };
		static constexpr DDL_BufferType GEX_GeometryNode{ GEX_Color + 1 };
		static constexpr DDL_BufferType GEX_GeometryObject{ GEX_GeometryNode + 1 };
		static constexpr DDL_BufferType GEX_IndexArray{ GEX_GeometryObject + 1 };
		static constexpr DDL_BufferType GEX_Key{ GEX_IndexArray + 1 };
		static constexpr DDL_BufferType GEX_LightNode{ GEX_Key + 1 };
		static constexpr DDL_BufferType GEX_LightObject{ GEX_LightNode + 1 };
		static constexpr DDL_BufferType GEX_Material{ GEX_LightObject + 1 };
		static constexpr DDL_BufferType GEX_MaterialRef{ GEX_Material + 1 };
		static constexpr DDL_BufferType GEX_Mesh{ GEX_MaterialRef + 1 };
		static constexpr DDL_BufferType GEX_Metric{ GEX_Mesh + 1 };
		static constexpr DDL_BufferType GEX_Morph{ GEX_Metric + 1 };
		static constexpr DDL_BufferType GEX_MorphWeight{ GEX_Morph + 1 };
		static constexpr DDL_BufferType GEX_Name{ GEX_MorphWeight + 1 };
		static constexpr DDL_BufferType GEX_Node{ GEX_Name + 1 };
		static constexpr DDL_BufferType GEX_ObjectRef{ GEX_Node + 1 };
		static constexpr DDL_BufferType GEX_Param{ GEX_ObjectRef + 1 };
		static constexpr DDL_BufferType GEX_Rotation{ GEX_Param + 1 };
		static constexpr DDL_BufferType GEX_Scale{ GEX_Rotation + 1 };
		static constexpr DDL_BufferType GEX_Skeleton{ GEX_Scale + 1 };
		static constexpr DDL_BufferType GEX_Skin{ GEX_Skeleton + 1 };
		static constexpr DDL_BufferType GEX_Spectrum{ GEX_Skin + 1 };
		static constexpr DDL_BufferType GEX_Texture{ GEX_Spectrum + 1 };
		static constexpr DDL_BufferType GEX_Time{ GEX_Texture + 1 };
		static constexpr DDL_BufferType GEX_Track{ GEX_Time + 1 };
		static constexpr DDL_BufferType GEX_Transform{ GEX_Track + 1 };
		static constexpr DDL_BufferType GEX_Translation{ GEX_Transform + 1 };
		static constexpr DDL_BufferType GEX_Value{ GEX_Translation + 1 };
		static constexpr DDL_BufferType GEX_VertexArray{ GEX_Value + 1 };

		static constexpr DDL_BufferType NEXT_BUFFER_ID{ GEX_VertexArray + 1 };
	};
}

