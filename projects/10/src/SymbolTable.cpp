#include "SymbolTable.h"

std::set<char> symbols =
{
	'{', '}',
	'(', ')',
	'[', ']',
	'.', ',', ';',
	'+', '-', '*', '/',
	'&', '|',
	'<', '>', '=',
	'~'
};

/// <summary>
/// Returns information if passed character is symbol.
/// </summary>
bool SymbolTable::isSymbol(char symbol)
{
	return symbols.count(symbol) != 0;
}