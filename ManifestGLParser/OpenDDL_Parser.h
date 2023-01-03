#pragma once
#include <vector>
#include <sstream>
#include <map>
#include <functional>

#include <ManifestGLUtility/DebugLogger.h>

#include "OpenDDL/DDL_File.h"
#include "RegisteredGenerators.h"


#include "FileFilter.h"


namespace Manifest_Parser
{	
	std::string ParseStructureHeader(const std::string& partitionedStructure, DDL_Structure& structure);
	//partitions a string of properties into a proeprty list for structures that include one
	PropertyList PartitionStructureProperties(const std::string& propertyList);
	//partitons the csv reference list into individual names
	ReferenceList PartitionStructureReferences(const std::string& partitionedStructure);
	//split ddl structures into separate objects - works by counting scope depth
	std::string PartitionDDLStructures(const std::string& filteredFile, size_t& filterOffset);
	//partition all structure in a file 
	ScratchPadVector<ScratchPadString> PartitionDDLFile(const std::string& filteredFile);
	//partition a previously partitioned structure into its substructures
	ScratchPadVector<std::string> PartitionDDLSubStructures(const std::string& partitionedStructure);
	const std::string LoadFileContents(const std::string& fileName);
	void ParseDDLFile(const ScratchPadVector<ScratchPadString>& fileContents, DDL_File& file);
}
