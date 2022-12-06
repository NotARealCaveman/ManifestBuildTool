#include "OpenGEX_Parser.h"

using namespace Manifest_Parser;

void Manifest_Parser::Initialize_GEXGenerators()
{	
	//init generators
	RegisteredGenerator::registeredGenerators.insert({ "Metric",new Generator<GEX_Metric> });
	RegisteredGenerator::registeredGenerators.insert({ "GeometryNode",new Generator<GEX_GeometryNode> });
	RegisteredGenerator::registeredGenerators.insert({ "GeometryObject",new Generator<GEX_GeometryObject> });
	RegisteredGenerator::registeredGenerators.insert({ "Material",new Generator<GEX_Material> });
}

void Manifest_Parser::InitializeExtendedTypes()
{
	//internally these are all float buffers and the subscript identifier will parse the type
	DDL_BufferTypes::DDL_BufferTypeMap.insert({ "float[2]",DDL_BufferTypes::DDL_float });
	DDL_BufferTypes::DDL_BufferTypeMap.insert({ "float[3]",DDL_BufferTypes::DDL_float });
	DDL_BufferTypes::DDL_BufferTypeMap.insert({ "float[4]",DDL_BufferTypes::DDL_float });
	DDL_BufferTypes::DDL_BufferTypeMap.insert({ "float[12]",DDL_BufferTypes::DDL_float });
}


void Manifest_Parser::Initialize_GEXTypes()
{
	InitializeExtendedTypes();
	DDL_BufferTypes::DDL_BufferTypeMap.insert({ "Animation", GEX_BufferTypes::GEX_Animation });
	DDL_BufferTypes::DDL_BufferTypeMap.insert({ "Atten", GEX_BufferTypes::GEX_Atten });
	DDL_BufferTypes::DDL_BufferTypeMap.insert({ "BoneCountArray", GEX_BufferTypes::GEX_BoneCountArray });
	DDL_BufferTypes::DDL_BufferTypeMap.insert({ "BoneIndexArray", GEX_BufferTypes::GEX_BoneIndexArray });
	DDL_BufferTypes::DDL_BufferTypeMap.insert({ "BoneNode",	GEX_BufferTypes::GEX_BoneNode });
	DDL_BufferTypes::DDL_BufferTypeMap.insert({ "BoneRefArray", GEX_BufferTypes::GEX_BoneRefArray });
	DDL_BufferTypes::DDL_BufferTypeMap.insert({ "BoneWeightArray", GEX_BufferTypes::GEX_BoneWeightArray });
	DDL_BufferTypes::DDL_BufferTypeMap.insert({ "CameraNode", GEX_BufferTypes::GEX_CameraNode });
	DDL_BufferTypes::DDL_BufferTypeMap.insert({ "CameraObject", GEX_BufferTypes::GEX_CameraObject });
	DDL_BufferTypes::DDL_BufferTypeMap.insert({ "Clip",GEX_BufferTypes::GEX_Clip });
	DDL_BufferTypes::DDL_BufferTypeMap.insert({ "Color", GEX_BufferTypes::GEX_Color });
	DDL_BufferTypes::DDL_BufferTypeMap.insert({ "GeometryNode", GEX_BufferTypes::GEX_GeometryNode });
	DDL_BufferTypes::DDL_BufferTypeMap.insert({ "GeometryObject", GEX_BufferTypes::GEX_GeometryObject });
	DDL_BufferTypes::DDL_BufferTypeMap.insert({ "IndexArray", GEX_BufferTypes::GEX_IndexArray });
	DDL_BufferTypes::DDL_BufferTypeMap.insert({ "Key",GEX_BufferTypes::GEX_Key });
	DDL_BufferTypes::DDL_BufferTypeMap.insert({ "LightNode", GEX_BufferTypes::GEX_LightNode });
	DDL_BufferTypes::DDL_BufferTypeMap.insert({ "LightObject", GEX_BufferTypes::GEX_LightObject });
	DDL_BufferTypes::DDL_BufferTypeMap.insert({ "Material", GEX_BufferTypes::GEX_Material });
	DDL_BufferTypes::DDL_BufferTypeMap.insert({ "MaterialRef", GEX_BufferTypes::GEX_MaterialRef });
	DDL_BufferTypes::DDL_BufferTypeMap.insert({ "Mesh",GEX_BufferTypes::GEX_Mesh });
	DDL_BufferTypes::DDL_BufferTypeMap.insert({ "Metric", GEX_BufferTypes::GEX_Metric });
	DDL_BufferTypes::DDL_BufferTypeMap.insert({ "Morph", GEX_BufferTypes::GEX_Morph });
	DDL_BufferTypes::DDL_BufferTypeMap.insert({ "MorphWeight", GEX_BufferTypes::GEX_MorphWeight });
	DDL_BufferTypes::DDL_BufferTypeMap.insert({ "Name", GEX_BufferTypes::GEX_Name });
	DDL_BufferTypes::DDL_BufferTypeMap.insert({ "Node",	GEX_BufferTypes::GEX_Node });
	DDL_BufferTypes::DDL_BufferTypeMap.insert({ "ObjectRef", GEX_BufferTypes::GEX_ObjectRef });
	DDL_BufferTypes::DDL_BufferTypeMap.insert({ "Param", GEX_BufferTypes::GEX_Param });
	DDL_BufferTypes::DDL_BufferTypeMap.insert({ "Rotation", GEX_BufferTypes::GEX_Rotation });
	DDL_BufferTypes::DDL_BufferTypeMap.insert({ "Scale", GEX_BufferTypes::GEX_Scale });
	DDL_BufferTypes::DDL_BufferTypeMap.insert({ "Skeleton", GEX_BufferTypes::GEX_Skeleton });
	DDL_BufferTypes::DDL_BufferTypeMap.insert({ "Skin", GEX_BufferTypes::GEX_Skin });
	DDL_BufferTypes::DDL_BufferTypeMap.insert({ "Spectrum", GEX_BufferTypes::GEX_Spectrum });
	DDL_BufferTypes::DDL_BufferTypeMap.insert({ "Texture", GEX_BufferTypes::GEX_Texture });
	DDL_BufferTypes::DDL_BufferTypeMap.insert({ "Time", GEX_BufferTypes::GEX_Time });
	DDL_BufferTypes::DDL_BufferTypeMap.insert({ "Track", GEX_BufferTypes::GEX_Track });
	DDL_BufferTypes::DDL_BufferTypeMap.insert({ "Transform", GEX_BufferTypes::GEX_Transform });
	DDL_BufferTypes::DDL_BufferTypeMap.insert({ "Translation", GEX_BufferTypes::GEX_Translation });
	DDL_BufferTypes::DDL_BufferTypeMap.insert({ "Value", GEX_BufferTypes::GEX_Value });
	DDL_BufferTypes::DDL_BufferTypeMap.insert({ "VertexArray", GEX_BufferTypes::GEX_VertexArray });
}