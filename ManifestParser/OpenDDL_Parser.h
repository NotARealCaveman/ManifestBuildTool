#pragma once
#include <vector>
#include <sstream>
#include <string_view>
#include <map>
#include <functional>

#include <ManifestUtility/DebugLogger.h>

#include "OpenDDL/DDL_File.h"
#include "RegisteredGenerators.h"


#include "FileFilter.h"


namespace Manifest_Parser
{		
	//partitons the csv reference list into individual names
	ReferenceList PartitionStructureReferences(const std::string_view& partitionedStructure);
	
	//load file first
	//ten partition file
	//then parse the file
	//partition in structures
	//for each structure, parse the header
	//partition properties
	//partition substructures and repeat
	
	const std::string LoadFileContents(const std::string& fileName);	
	std::vector<std::string> PartitionDDLFile(const std::string& filteredFile);
	void ParseDDLFile(const std::vector<std::string>& fileContents, DDL_File& file);
	//split ddl structures into separate objects - works by counting scope depth
	std::string_view PartitionDDLStructures(const std::string_view& filteredFile, size_t& filterOffset);
	//header: name & type, properties
	std::string_view ParseStructureHeader(const std::string_view& partitionedStructureView, DDL_Structure& structure);
	//partitions a stringview of properties into a property list for structures that include one
	PropertyList PartitionStructureProperties(const std::string_view& propertyListView);
	//partition a previously partitioned structure into its substructures
	ScratchPadVector<std::string_view> PartitionDDLSubStructures(const std::string_view& partitionedStructureView);	
}
