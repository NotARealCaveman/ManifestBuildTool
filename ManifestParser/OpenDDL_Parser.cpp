#include "OpenDDL_Parser.h"

using namespace Manifest_Parser;

//sets the identifier and name of the ddl structure
std::string_view Manifest_Parser::ParseStructureHeader(const std::string_view& partitionedStructureView, DDL_Structure& structure)
{
	auto payloadIdentifier = partitionedStructureView.find_first_of('{');
	const auto header = partitionedStructureView.substr(0, payloadIdentifier);
	auto nameIdentifier = header.find_first_of("$%");
	auto propertyIdentifier = header.find_first_of('(');	
	structure.identifier = header.substr(0, std::min(nameIdentifier, std::min(propertyIdentifier, payloadIdentifier)));
	structure.name = nameIdentifier == std::string::npos ? "" : header.substr(nameIdentifier, std::min(propertyIdentifier, payloadIdentifier) - nameIdentifier);
	auto result{ propertyIdentifier == std::string::npos ? "" : header.substr(propertyIdentifier,payloadIdentifier) };

	return result;
}

//sets all the asupplied optional properties of the structure
PropertyList Manifest_Parser::PartitionStructureProperties(const std::string_view& propertyListView)
{	
	auto temp = propertyListView;
	auto next = temp.find_first_not_of('(');
	if (next == std::string::npos)
		return {};	
	auto end = temp.find_first_of(",)");
	PropertyList result;
	while(temp[next] != ')')
	{
		auto equator = temp.find_first_of('=');		
		result.emplace_back(DDL_Property{ temp.substr(next, equator - next),temp.substr(equator + 1, end - (equator + 1)) });
		temp = temp.substr(end);
		next = temp.find_first_not_of(',');
		end = temp.find_first_of(",)",next);
	}

	return result;
}

ReferenceList Manifest_Parser::PartitionStructureReferences(const std::string_view& partitionedStructure)
{	
	ReferenceList result;	
	auto referenceList{ PartitionDDLSubStructures(partitionedStructure) };
	for (const auto& reference : referenceList)
	{
		auto begin = reference.find_first_of('$');
		auto end = reference.find_last_of('}');
		const auto& referenceName{ reference.substr(begin,end - begin) };
		result.emplace_back(referenceName);		
	}
	return result;
}

//starting at the first identifier begins counting scope depths to determine when a sutrcture has exhausted its content 
std::string_view Manifest_Parser::PartitionDDLStructures(const std::string_view& filteredFile, size_t& filterOffset)
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


//performs the structuer partition on an entire file o
std::vector<std::string> Manifest_Parser::PartitionDDLFile(const std::string& filteredFile)
{
	std::vector<std::string> result;//list of top level structures to be generated	
	std::string file = filteredFile;
	size_t offset = 0;
	for (; *file.begin() != static_cast<char>(0); file = file.substr(offset))
		result.emplace_back(PartitionDDLStructures(file, offset));		
	
	return  result;
}

ScratchPadVector<std::string_view> Manifest_Parser::PartitionDDLSubStructures(const std::string_view& partitionedStructureView)
{
	auto begin = partitionedStructureView.find_first_of('{') + 1;
	auto end = partitionedStructureView.find_last_of('}');
	auto payload = partitionedStructureView.substr(begin, end - begin);
	ScratchPadVector<std::string_view> result;
	if (payload.find_first_of("{") != std::string::npos)	
		for(size_t offset = 0; !payload.empty(); payload = payload.substr(offset))
			result.emplace_back(PartitionDDLStructures(payload, offset));
	else 
		result.emplace_back(payload);

	return result;
}

const std::string Manifest_Parser::LoadFileContents(const std::string& fileName)
{	
	auto fileStream = std::fstream{ fileName,std::ios::in };
	std::string result;
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

void Manifest_Parser::ParseDDLFile(const std::vector<std::string>& fileContents, DDL_File& fileObject)
{		
	for (const auto& structure : fileContents)
	{		
		auto identifier = structure.substr(0, structure.find_first_of("$%({"));
		auto registeredGenerator = RegisteredGenerator::registeredGenerators.find(identifier);
		auto generator = registeredGenerator->second;
		DDL_Structure* primaryStructure = nullptr;
		//does implicitly cast to string_view but static_casted here for clarity
		primaryStructure = generator->GenerateType(static_cast<std::string>(structure), fileObject.referenceMap);
		fileObject.primaryStructures.push_back(primaryStructure);
	}
}