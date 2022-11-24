#pragma once 
#include <map>
#include <string>
#include <vector>

#include <ManifestGLPersistence/DatabaseTypes.h>

namespace Manifest_Persistence
{	
	template<typename MDB_TableType>
	struct DatabaseTable
	{
		std::map<std::string,PrimaryKey> mappedEntryKeys;
		std::vector<MDB_TableType> entries;
		PrimaryKey nextTableIndex = 0;
	};

	//function placed at the top level in Manifest_Table.h so that the binaries which include the mdb types may have access to the function. binary types do not know about the table and therefore both need to have a high level hand off of this function. 
	template<typename Binary_TableType>
	inline size_t EntrySize(const Binary_TableType& entry)
	{
		return sizeof(Binary_TableType) + entry.header.payloadSize;
	}
}