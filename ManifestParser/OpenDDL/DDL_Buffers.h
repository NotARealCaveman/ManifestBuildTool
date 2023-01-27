#pragma once
#include <map>
#include <string>
#include <vector>

#include <ManifestParser/OpenDDL_Parser.h>

namespace Manifest_Parser
{
	
	typedef uint32_t DDL_BufferType;	
	//returns the ddl type based on the identifier	
	DDL_BufferType ExtractStructureType(const std::string& partitionedStructure);

	struct DDL_BufferTypes
	{
		static constexpr DDL_BufferType DDL_ref{ 0 };
		static constexpr DDL_BufferType DDL_int8{ DDL_ref + 1 };
		static constexpr DDL_BufferType DDL_int16{ DDL_int8 + 1 };
		static constexpr DDL_BufferType DDL_int32{ DDL_int16 + 1 };
		static constexpr DDL_BufferType DDL_int64{ DDL_int32 + 1 };
		static constexpr DDL_BufferType DDL_uint8{ DDL_int64 + 1 };
		static constexpr DDL_BufferType DDL_uint16{ DDL_uint8 + 1 };
		static constexpr DDL_BufferType DDL_uint32{ DDL_uint16 + 1 };
		static constexpr DDL_BufferType DDL_uint64{ DDL_uint32 + 1 };
		static constexpr DDL_BufferType DDL_half{ DDL_uint64 + 1 };
		static constexpr DDL_BufferType DDL_float{ DDL_half + 1 };
		static constexpr DDL_BufferType DDL_double{ DDL_float + 1 };
		static constexpr DDL_BufferType DDL_string{ DDL_double + 1 };
		//reserve for extended types - extension will supply a next buffer for further file formats
		static constexpr DDL_BufferType NEXT_BUFFER_ID = DDL_string + 1;
		//Maps literal types to their ddl symbols
		static std::map<std::string, DDL_BufferType> DDL_BufferTypeMap;
	};	

	struct DDL_Buffer
	{
		void* typeHeap = nullptr;
		DDL_BufferType bufferType;//buffer type will only ever be of the original primitive
		size_t subBufferElements;//number of elements per sub buffer
		size_t subBufferCount;//number of sub buffers
	};
	
	//buffer is converted to type T then offset with count of Ts
	template<typename Type>
	const Type* BufferData(const DDL_Buffer& buffer, const uint32_t offset = 0)
	{
		return reinterpret_cast<Type*>(buffer.typeHeap)+offset;
	}

	//prepares sub buffer data - returns the number of elements per sub buffer
	//allocates room for buffer data on buffer::typeHeap
	template<typename T>
	size_t PrepareSubBuffer(const std::string& partitionedStructure,const size_t& bufferCount ,DDL_Buffer& buffer)
	{
		auto beginSubBuffer = partitionedStructure.find_first_of('[');
		auto endSubBuffer = partitionedStructure.find_first_of(']');						
		buffer.subBufferElements = std::stoi(partitionedStructure.substr(beginSubBuffer + 1, endSubBuffer - beginSubBuffer - 1));
		buffer.subBufferCount = bufferCount;
		//buffer.typeHeap = new T[buffer.subBufferCount * buffer.subBufferElements];
		buffer.typeHeap = New<T,ScratchPad<T>>(buffer.subBufferCount * buffer.subBufferElements);

		return buffer.subBufferElements;
	}

	//fills a sub buffer array with the per substructure extracted from payload information
	template<typename T>
	void ExtractSubBuffer(const ScratchPadVector<std::string> & subBufferData,const size_t& subBufferElementCount, const DDL_Buffer& buffer)
	{
		uint32_t subBuffer = 0;
		for (const auto& bufferData : subBufferData)
		{
			//DLOG(0, bufferData);
			auto payloadIndex = bufferData.find_first_of("{");
			auto payload = bufferData.substr(payloadIndex + 1, bufferData.find_first_of('}') - payloadIndex - 1);
			for (uint32_t bufferPosition = 0; bufferPosition < subBufferElementCount; ++bufferPosition)
			{
				T temp = static_cast<T>(std::stof
				(payload.substr(0)));				
				*(reinterpret_cast<T*>(buffer.typeHeap) + subBuffer++) = temp;				
				payload = payload.substr(payload.find_first_of(',') + 1);
			}
		}
	}

	template<typename T>
	void BuildSubBuffer(const std::string& partitionedStructure, DDL_Buffer& buffer)
	{
		ScratchPadVector<std::string> subBufferData = PartitionDDLSubStructures(partitionedStructure);
		size_t subBufferElementCount = PrepareSubBuffer<T>(partitionedStructure, subBufferData.size(), buffer);
		ExtractSubBuffer<T>(subBufferData, subBufferElementCount, buffer);
	}
}