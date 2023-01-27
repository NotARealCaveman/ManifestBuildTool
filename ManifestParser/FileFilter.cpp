#include "FileFilter.h"

using namespace Manifest_Parser;

std::string Manifest_Parser::FilterWhiteSpace(const std::string& unfiltered)
{
	auto result = unfiltered;
	result.erase(std::remove(result.begin(), result.end(), ' '), result.end());
	result.erase(std::remove(result.begin(), result.end(), '\t'), result.end());
	return result;
}

std::string Manifest_Parser::FilterComments(const std::string& unfiltered)
{
	std::string result, streamData;
	std::stringstream streamFilter{ unfiltered };
	bool mlc = false;//multi-line comment
	//remove all multiline commens
	while (std::getline(streamFilter, streamData))
	{
		auto index = streamData.find("/*");
		mlc = index == std::string::npos ? false : true;
		auto length = index == std::string::npos ? streamData.length() : index;
		result += streamData.substr(0, length)+'\n';
		auto temp = streamData;
		while (mlc)
		{
			index = temp.find("*/");//+2			
			mlc = index == std::string::npos ?
				[&]()->bool {temp.clear();std::getline(streamFilter, temp);	return true; }():
				[&]()->bool {streamData = temp.substr(index + 2); result += streamData + '\n'; return false; }();
		}		
	}	
	//remove all single line comments
	streamData.clear();	
	std::string temp{ "" };
	std::swap(result, temp);	
	streamFilter = std::stringstream{ temp };
	while (std::getline(streamFilter, streamData))
	{
		auto index = streamData.find("//");
		index = index == std::string::npos ? streamData.length() : index;
		result += streamData.substr(0, index);
		streamData.clear();
	}

	return result;
}

std::string Manifest_Parser::FilterNewLines(const std::string& unfiltered)
{
	
	std::string result = unfiltered;
	result.erase(std::remove(result.begin(), result.end(), '\n'), result.end());
	return result;
}
//remove all comments->newlines->white spaces			
std::string Manifest_Parser::FilterFile(const std::string& unfiltered, const uint8_t& filterFlags)
{
	std::string filtered = unfiltered;
	if (filterFlags & COMMENT_FILTER)
		filtered = FilterComments(filtered);
	if (filterFlags & NEWLINE_FILTER)
		filtered = FilterNewLines(filtered);//needs to be reworked to not interfere with strings
	if (filterFlags & WHITESPACE_FILTER)
		filtered = FilterWhiteSpace(filtered);//needs to be reworked to not interfere with strings
	return 	filtered;
}