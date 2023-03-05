#include "DDL_Buffers.h"

using namespace Manifest_Parser;

std::map<std::string, DDL_BufferType> DDL_BufferTypes::DDL_BufferTypeMap
{
	
	//references
	{"ref",	DDL_BufferTypes::DDL_ref},
	//signed ints
	{"int8",	DDL_BufferTypes::DDL_int8},	{ "i8",			DDL_BufferTypes::DDL_int8 },
	{"int16",	DDL_BufferTypes::DDL_int16 },	{ "i16",		DDL_BufferTypes::DDL_int16 },
	{"int32",	DDL_BufferTypes::DDL_int32 },	{ "i32",		DDL_BufferTypes::DDL_int32 },
	{"int64",	DDL_BufferTypes::DDL_int64 },	{ "i64",		DDL_BufferTypes::DDL_int64 },
	//unsigned ints
	{"uint8",	DDL_BufferTypes::DDL_uint8},	{ "u8",			DDL_BufferTypes::DDL_uint8 },
	{"uint16",	DDL_BufferTypes::DDL_uint16 },	{ "u16",		DDL_BufferTypes::DDL_uint16 },
	{"uint32",	DDL_BufferTypes::DDL_uint32 },	{ "u32",		DDL_BufferTypes::DDL_uint32 },
	{"uint64",	DDL_BufferTypes::DDL_uint64 },	{ "u64",		DDL_BufferTypes::DDL_uint64 },	
	//floats
	{"half",	DDL_BufferTypes::DDL_half },	{ "float16",	DDL_BufferTypes::DDL_half },
	{"h",		DDL_BufferTypes::DDL_half },	{ "f16",		DDL_BufferTypes::DDL_half },
	{"float",	DDL_BufferTypes::DDL_float },	{ "float32",	DDL_BufferTypes::DDL_float },
	{"f",		DDL_BufferTypes::DDL_float },	{ "f32",		DDL_BufferTypes::DDL_float },
	{"double",	DDL_BufferTypes::DDL_double },	{ "float64",	DDL_BufferTypes::DDL_double },
	{"d",		DDL_BufferTypes::DDL_double },	{ "f64",		DDL_BufferTypes::DDL_double },
	//strings - symbols tbd
	{ "string",DDL_BufferTypes::DDL_string }
};

DDL_BufferType Manifest_Parser::ExtractStructureType(const std::string_view& partitionedStructure)

{
	auto identifier = partitionedStructure.substr(0, partitionedStructure.find_first_of("$%({"));//dont include [ as sub arrays may be defined in the extended types
	auto typeMap = DDL_BufferTypes::DDL_BufferTypeMap.find(static_cast<std::string>(identifier));
	return typeMap->second;
}
