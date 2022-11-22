#pragma once
#include <fstream>

#include "Binary_Mesh.h"
#include "Binary_GeometryObject.h"

namespace Manifset_Persistence
{
	//total table size is sz(Header)+sz(Entries),
	//where Entries = n*sz(entry)+i:[0,n]Σsz(entry:Payload[i])		
	template<typename Binary_TableType>
	struct BinaryTable
	{
		struct Table_Header
		{
			size_t totalEntries;//number of base elements to be created		
			size_t dynamicTableSize{ 0 };//number of bytes to be created for elements + payloads
		}header;
		Binary_TableType* entries;
		Binary_TableType const* const operator[](const int32_t& index)
		{
			Binary_TableType* result = nullptr;
			if (index < 0 || index > header.totalEntries - 1)
				return nullptr;

			size_t byteOffset{ 0 };
			for (int32_t entry = 0; entry <= index; ++entry)
			{
				auto tableAddress = reinterpret_cast<char*>(entries) + byteOffset;
				result = reinterpret_cast<Binary_TableType*>(tableAddress);
				byteOffset += sizeof(Binary_TableType) + result->header.payloadSize;
			}
			return result;
		};
	};


	template<typename Binary_TableType, typename MDB_Table, typename... Args>
	BinaryTable<Binary_TableType> BuildBinaryTable(const MDB_Table& mdb_Table, const Args&... args)
	{
		BinaryTable<Binary_TableType> result;
		result.entries = new Binary_TableType[result.header.totalEntries = mdb_Table.entries.size()];
		for (auto entry = 0; entry < result.header.totalEntries; ++entry)
			result.header.dynamicTableSize += Convert_MDB(mdb_Table.entries[entry], args..., result.entries[entry]);
		return result;
	}

	//header is extracted first then room for all entries is extracted	
	template<typename Binary_TableType>
	void ExportBinaryTable(const BinaryTable<Binary_TableType> table, std::ofstream& currentExport)
	{
		currentExport.write(reinterpret_cast<const char*>(&table.header), sizeof(BinaryTable<Binary_TableType>::Table_Header));
		for (auto entry = 0; entry < table.header.totalEntries; ++entry)
		{
			const Binary_TableType& record{ table.entries[entry] };
			currentExport.write(reinterpret_cast<const char*>(&record.header), sizeof(record.header));//write header data
			currentExport.write("\0\0\0\0\0\0\0\0", sizeof(Binary_TableType*));//write storage for payload ptr - nullptr
			currentExport.write(reinterpret_cast<const char*>(record.payload), record.header.payloadSize);//write payload
		}
	}

	template<typename Binary_TableType>
	BinaryTable<Binary_TableType> ImportBinaryTable(std::ifstream& currentImport)
	{
		BinaryTable<Binary_TableType> result;

		currentImport.read(reinterpret_cast<char*>(&result.header), sizeof(BinaryTable<Binary_TableType>::Table_Header));
		const size_t& allocation = result.header.dynamicTableSize;

		result.entries = reinterpret_cast<Binary_TableType*>(malloc(allocation));
		currentImport.read(reinterpret_cast<char*>(result.entries), allocation);
		uint64_t byteOffset{ 0 };
		for (auto entry = 0; entry < result.header.totalEntries; ++entry)
		{
			char* tableAddress = reinterpret_cast<char*>(result.entries) + byteOffset;//move to current table header
			Binary_TableType& record = *reinterpret_cast<Binary_TableType*>(tableAddress);
			byteOffset += sizeof(Binary_TableType);//align payload pointer with payload
			tableAddress = reinterpret_cast<char*>(result.entries) + byteOffset;//move to payload
			record.payload = reinterpret_cast<decltype(record.payload)>(tableAddress);//pointer is now neighbor to payload
			byteOffset += record.header.payloadSize;
		}
		return result;
	}
}