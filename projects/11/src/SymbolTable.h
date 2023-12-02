#pragma once
#include <string>
#include <vector>
#include "ESymbolKind.h"
#include "Symbol.h"

/// <summary>
/// SymbolTable is responsible for building, populating and using symbols.
/// </summary>
class SymbolTable
{
public:
    /// <summary>
    /// Defines (adds to the table) a new variable of the given name, type, and kind.
    /// Assings to it the index value of that kind, and adds 1 to the index.
    /// </summary>
    void define(std::string name, std::string type, ESymbolKind kind);
    /// <summary>
    /// Defines (adds to the table) a new variable of the given name, type, and kind.
    /// Assings to it the specified index, and adds 1 to the index.
    /// </summary>
    void define(std::string name, std::string type, ESymbolKind kind, int index);
    /// <summary>
    /// Returns the number of variables of the given kind already defined in the table.
    /// </summary>
    int varCount(ESymbolKind kind);
    /// <summary>
    /// Retrieves information about a symbol with the given name from the symbol table.
    /// If the symbol is not found, returns a Symbol with default values.
    /// </summary>
    /// <param name="name">The name of the symbol to retrieve.</param>
    /// <returns>A Symbol struct containing information about the symbol.</returns>
    Symbol getSymbol(std::string name);

private:
    std::vector<std::pair<std::string, std::string>>& getVector(ESymbolKind symbolKind);
    int findSymbol(const std::vector<std::pair<std::string, std::string>>& symbols, const std::string& key);

    std::vector<std::pair<std::string, std::string>> staticSymbols;
    std::vector<std::pair<std::string, std::string>> fieldSymbols;
    std::vector<std::pair<std::string, std::string>> argSymbols;
    std::vector<std::pair<std::string, std::string>> varSymbols;
};
