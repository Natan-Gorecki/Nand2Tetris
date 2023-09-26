#pragma once
#include <set>
#include <string>

/// <summary>
/// Contains reference of all symbols for the Jack language.
/// </summary>
class SymbolTable
{
public:
	/// <summary>
	/// Returns information if passed character is symbol.
	/// </summary>
	static bool isSymbol(char symbol);
};
