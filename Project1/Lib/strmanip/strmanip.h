#pragma once
#include <unordered_map>
#include <memory>
#include <string>
namespace strmanip
{
	// Extract The String
	//@pram:lineString	Strings used for research
	//@pram:stringId	Checks for stringID strings in linestring
	std::string ExtractTheStrDblQt(std::string lineString, std::string stringId);
}