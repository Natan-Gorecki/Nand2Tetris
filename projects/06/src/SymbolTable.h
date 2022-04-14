#pragma once
#include <string>

class SymbolTable
{
public:
	/// <summary>
	/// Adds <symbol,address> to the table.
	/// </summary>
	void addEntry(std::string symbol, int address);
	/// <summary>
	/// Does the symbol table contain the given symbol?
	/// </summary>
	bool contains(std::string symbol);
	/// <summary>
	/// Returns the address associated with the symbol.
	/// </summary>
	int getAddress(std::string symbol);

public:
	/// <summary>
	/// Creates a new empty symbol table.
	/// </summary>
	SymbolTable();
};

