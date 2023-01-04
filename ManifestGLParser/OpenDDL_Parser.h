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
	ScratchPadString ParseStructureHeader(const ScratchPadString& partitionedStructure, DDL_Structure& structure);
	//partitions a string of properties into a proeprty list for structures that include one
	PropertyList PartitionStructureProperties(const ScratchPadString& propertyList);
	//partitons the csv reference list into individual names
	ReferenceList PartitionStructureReferences(const ScratchPadString& partitionedStructure);
	//split ddl structures into separate objects - works by counting scope depth
	ScratchPadString PartitionDDLStructures(const ScratchPadString& filteredFile, size_t& filterOffset);
	//partition all structure in a file 
	ScratchPadVector<ScratchPadString> PartitionDDLFile(const ScratchPadString& filteredFile);
	std::vector<std::string> PartitionDDLFile(const std::string& filteredFile);
	//partition a previously partitioned structure into its substructures
	ScratchPadVector<ScratchPadString> PartitionDDLSubStructures(const ScratchPadString& partitionedStructure);
	const ScratchPadString LoadFileContents(const std::string& fileName);
	void ParseDDLFile(const std::string& fileName, DDL_File& file);	
	ScratchPadVector<std::string> PartitionDDLSubStructures(const std::string& partitionedStructure);
	const std::string LoadFileContents(const std::string& fileName);
	void ParseDDLFile(const std::vector<std::string>& fileContents, DDL_File& file);

	//testing allocations
	ScratchPadString PartitionDDLStructuresV2(const ScratchPadString& filteredFile, size_t& filterOffset);
	ScratchPadVector<ScratchPadString> PartitionDDLSubStructuresV2(const ScratchPadString& partitionedStructure);
}
