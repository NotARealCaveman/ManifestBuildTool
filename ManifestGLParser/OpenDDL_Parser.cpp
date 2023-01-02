#include "OpenDDL_Parser.h"

using namespace Manifest_Parser;

ScratchPadString Manifest_Parser::ParseStructureHeader(const ScratchPadString& partitionedStructure, DDL_Structure& structure)
{
	auto payloadIdentifier = partitionedStructure.find_first_of('{');
	const ScratchPadString header = partitionedStructure.substr(0, payloadIdentifier);
	auto nameIdentifier = header.find_first_of("$%");
	auto propertyIdentifier = header.find_first_of('(');	
	structure.identifier = header.substr(0, std::min(nameIdentifier, std::min(propertyIdentifier, payloadIdentifier)));
	structure.name = nameIdentifier == ScratchPadString::npos ? "" : header.substr(nameIdentifier, std::min(propertyIdentifier, payloadIdentifier) - nameIdentifier);
	ScratchPadString result{ propertyIdentifier == std::string::npos ? "" : header.substr(propertyIdentifier,payloadIdentifier) };

	return result;
}

PropertyList Manifest_Parser::PartitionStructureProperties(const ScratchPadString& properties)
{	
	auto temp = properties;
	auto next = temp.find_first_not_of('(');
	if (next == std::string::npos)
		return {};	
	auto end = temp.find_first_of(",)");
	PropertyList result;
	while(temp[next] != ')')
	{
		auto equator = temp.find_first_of('=');		
		result.emplace_back(DDL_Property{ temp.substr(next, equator-next),temp.substr(equator + 1, end-(equator + 1)) });
		temp = temp.substr(end);
		next = temp.find_first_not_of(',');
		end = temp.find_first_of(",)",next);
	}

	return result;
}

ReferenceList Manifest_Parser::PartitionStructureReferences(const ScratchPadString& partitionedStructure)
{	
	auto begin = partitionedStructure.find_first_of("$%{(");
	auto end = partitionedStructure.find_last_of('}')+1;
	auto references = partitionedStructure.substr(begin, end - begin);		
	ReferenceList result;
	while (*references.begin() != '}')
	{		
		result.emplace_back(references.substr(1, (end = references.find_first_of(",}", 1)) - 1));
		references = references.substr(end);		
	}
	return result;
}

//starting at the first identifier begins counting scope depths to determine when a sutrcture has exhausted its content 
ScratchPadString Manifest_Parser::PartitionDDLStructures(const ScratchPadString& filteredFile, size_t& filterOffset)
{
	//determine end of structure by counting scope depth - once it reaches 0 then this is the	endpoint of the current sturcture and the file is split from that point
	auto temp = filteredFile;
	auto withinScope = true;
	size_t beginScope = temp.find_first_of('{');
	size_t endScope = temp.find_first_of('}');	
	uint32_t scopeDepth = 0;
	filterOffset = 0;
	while (withinScope)
	{		
		if (beginScope < endScope)
		{
			++scopeDepth;
			temp = temp.substr(beginScope + 1);
			filterOffset += beginScope + 1;
		}
		else
		{
			--scopeDepth;
			temp = temp.substr(endScope + 1);
			filterOffset += endScope + 1;
		}
		if (!scopeDepth)
			withinScope = false;
		beginScope = temp.find_first_of('{');
		endScope = temp.find_first_of('}');
	}

	return filteredFile.substr(0, filterOffset);
}

//performs the structuer partition on an entire file or previously partitioned structure
ScratchPadVector<ScratchPadString> Manifest_Parser::PartitionDDLFile(const ScratchPadString& filteredFile)
{
	ScratchPadVector<ScratchPadString> result;//list of top level structures to be generated	
	ScratchPadString file = filteredFile;
	size_t offset = 0;
	for (; *file.begin() != static_cast<char>(0); file = file.substr(offset))
		result.emplace_back(PartitionDDLStructures(file, offset));		
	
	return  result;
}

ScratchPadVector<ScratchPadString> Manifest_Parser::PartitionDDLSubStructures(const ScratchPadString& partitionedStructure)
{
	auto begin = partitionedStructure.find_first_of('{') + 1;
	auto end = partitionedStructure.find_last_of('}');
	auto payload = partitionedStructure.substr(begin, end - begin);
	ScratchPadVector<ScratchPadString> result;
	for(size_t offset = 0; !payload.empty(); payload = payload.substr(offset))
		result.emplace_back(PartitionDDLStructures(payload, offset));

	return result;
}

const ScratchPadString Manifest_Parser::LoadFileContents(const std::string& fileName)
{	
	auto fileStream = std::fstream{ fileName,std::ios::in };
	ScratchPadString result;
	if (fileStream.is_open())
	{
		fileStream.seekg(0, fileStream.end);
		int length = fileStream.tellg();
		fileStream.seekg(0, fileStream.beg);
		result.resize(length);
		fileStream.read(result.data(), length);
		fileStream.close();
	}
	return result;
}

void Manifest_Parser::ParseDDLFile(const std::string& fileName, DDL_File& fileObject)
{	
	for (const auto& structure : PartitionDDLFile(FilterFile(LoadFileContents(fileName))))
	{
		auto identifier = structure.substr(0, structure.find_first_of("$%({"));
		auto registeredGenerator = RegisteredGenerator::registeredGenerators.find(identifier);
		auto generator = registeredGenerator->second;
		fileObject.primaryStructures.push_back(generator->GenerateType(structure,fileObject.referenceMap));			
	}
}