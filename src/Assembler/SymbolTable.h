#pragma once

#include <map>
#include <string>

/// <summary>
/// Resolves symbolic references into actual addresses
/// </summary>
class SymbolTable
{
public:
    /// <summary>
    /// Adds <symbol,address> to the table.
    /// </summary>
    void addEntry(const std::string& symbol, int address);
    /// <summary>
    /// Does the symbol table contain the given symbol?
    /// </summary>
    bool contains(const std::string& symbol);
    /// <summary>
    /// Returns the address associated with the symbol.
    /// </summary>
    int getAddress(const std::string& symbol);

    /// <summary>
    /// Creates a new empty symbol table.
    /// </summary>
    SymbolTable();

private:
    std::map<std::string, int, std::less<>> mSymbolTable;
};
