#pragma once
#include <sstream>
#include <functional>

#include <ManifestGLUtility/DebugLogger.h>

namespace Manifest_Parser
{
	//filter file to prepare for parsing - to be moved out of ddl
	constexpr uint8_t NO_FILTER = 0;
	constexpr uint8_t COMMENT_FILTER = 1;
	constexpr uint8_t NEWLINE_FILTER = 2;
	constexpr uint8_t WHITESPACE_FILTER = 4;
	constexpr uint8_t ALL_FILTER = COMMENT_FILTER | NEWLINE_FILTER | WHITESPACE_FILTER;

	std::string FilterWhiteSpace(const std::string& unfiltered);
	std::string FilterComments(const std::string& unfiltered);
	std::string FilterNewLines(const std::string& unfiltered);
	std::string FilterFile(const std::string& unfiltered, const uint8_t& filterFlags = ALL_FILTER);
}