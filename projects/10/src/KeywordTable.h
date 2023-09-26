#pragma once
#include <set>
#include <string>

/// <summary>
/// Contains reference of all keywords for the Jack language.
/// </summary>
class KeywordTable
{
public:
	/// <summary>
	/// Returns information if passed string is a keyword.
	/// </summary>
	static bool isKeyword(std::string name);
};
