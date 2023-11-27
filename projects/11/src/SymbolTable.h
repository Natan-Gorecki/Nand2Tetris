#pragma once
#include <string>
#include <vector>
#include "ESymbolKind.h"

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
    /// Returns the kind of the named identifier.
    /// If the identifier is not found, returns UNDEFINED.
    /// </summary>
    ESymbolKind kindOf(std::string name);
    /// <summary>
    /// Returns the type of the named variable.
    /// </summary>
    std::string typeOf(std::string name);
    /// <summary>
    /// Returns the index of the named variable.
    /// </summary>
    int indexOf(std::string name);

private:
    std::vector<std::pair<std::string, std::string>>& getVector(ESymbolKind symbolKind);
    int findSymbol(const std::vector<std::pair<std::string, std::string>>& symbols, const std::string& key);

    std::vector<std::pair<std::string, std::string>> staticSymbols;
    std::vector<std::pair<std::string, std::string>> fieldSymbols;
    std::vector<std::pair<std::string, std::string>> argSymbols;
    std::vector<std::pair<std::string, std::string>> varSymbols;
};
