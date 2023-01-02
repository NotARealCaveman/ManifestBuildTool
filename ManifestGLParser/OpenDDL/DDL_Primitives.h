#pragma once
#include <string>

#include <ManifestGLParser/RegisteredGenerators.h>
#include <ManifestGLParser/OpenDDL_Parser.h>

#include "DDL_Buffers.h"

namespace Manifest_Parser
{

	template<typename T, DDL_BufferType bufferType>
	struct Primitive : public RegisteredBuilder
	{
		DDL_Structure Build(const std::string& partitionedStructure, DDL_ReferenceMap& referenceMap) final
		{			
			DDL_Structure result;
			ParseStructureHeader(partitionedStructure, result);
			data.bufferType = bufferType;
			//check for sub buffer data
			auto subBufferIdentifier = partitionedStructure.find_first_of("[");
			if (subBufferIdentifier == std::string::npos)
			{
				auto payloadIndex = partitionedStructure.find_first_of("{");
				data.subBufferElements = data.subBufferCount= 1;
				//make room for primitive buffer - will require an additional allocation from the original generation
				//data.typeHeap = new T;				
				data.typeHeap = New<T,ScratchPad<T>>(1);
				*reinterpret_cast<T*>(data.typeHeap) = static_cast<T>(std::stof(partitionedStructure.substr(payloadIndex + 1)));;				
			}
			else
				BuildSubBuffer<T>(partitionedStructure, data);
			result.typeHeap = static_cast<void*>(this);
			MapStructureName(result, referenceMap);

			return result;
		}
		DDL_Buffer data;
	};
	
	typedef Primitive<float, DDL_BufferTypes::DDL_float> DDL_Float;
	typedef Primitive<double, DDL_BufferTypes::DDL_double> DDL_Double;
	typedef Primitive<int8_t, DDL_BufferTypes::DDL_int8> DDL_Int8;
	typedef Primitive<int16_t, DDL_BufferTypes::DDL_int16> DDL_Int16;
	typedef Primitive<int32_t, DDL_BufferTypes::DDL_int32> DDL_Int32;
	typedef Primitive<int64_t, DDL_BufferTypes::DDL_int64> DDL_Int64;
	typedef Primitive<uint8_t, DDL_BufferTypes::DDL_uint8> DDL_Uint8;
	typedef Primitive<uint16_t, DDL_BufferTypes::DDL_uint16> DDL_Uint16;
	typedef Primitive<uint32_t, DDL_BufferTypes::DDL_uint32> DDL_Uint32;
	typedef Primitive<uint64_t, DDL_BufferTypes::DDL_uint64> DDL_Uint64;


	struct DDL_String : public RegisteredBuilder
	{
		DDL_Structure Build(const std::string& partitionedStructure, DDL_ReferenceMap& referenceMap) final;

		std::string data;
	};
}