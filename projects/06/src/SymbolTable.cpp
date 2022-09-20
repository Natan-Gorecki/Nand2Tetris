#include "SymbolTable.h"

/// <summary>
/// Adds <symbol,address> to the table.
/// </summary>
void SymbolTable::addEntry(std::string symbol, int address)
{
	symbol_table.insert_or_assign(symbol, address);
}

/// <summary>
/// Does the symbol table contain the given symbol?
/// </summary>
bool SymbolTable::contains(std::string symbol)
{
	std::map<std::string, int>::iterator it = symbol_table.find(symbol);
	return it != symbol_table.end();
}

/// <summary>
/// Returns the address associated with the symbol.
/// </summary>
int SymbolTable::getAddress(std::string symbol)
{
	std::map<std::string, int>::iterator it = symbol_table.find(symbol);
	if (it != symbol_table.end())
	{
		return it->second;
	}
	return 0;
}

/// <summary>
/// Creates a new empty symbol table.
/// </summary>
SymbolTable::SymbolTable()
{

}