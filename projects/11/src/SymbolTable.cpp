#include "JackCompilerError.h"
#include "SymbolTable.h"

/// <summary>
/// Defines (adds to the table) a new variable of the given name, type, and kind.
/// Assings to it the index value of that kind, and adds 1 to the index.
/// </summary>
void SymbolTable::define(std::string name, std::string type, ESymbolKind kind)
{
    auto& vector = getVector(kind);
    vector.push_back({ name, type });
}

/// <summary>
/// Defines (adds to the table) a new variable of the given name, type, and kind.
/// Assings to it the specified index, and adds 1 to the index.
/// </summary>
void SymbolTable::define(std::string name, std::string type, ESymbolKind kind, int index)
{
    auto& vector = getVector(kind);
    auto position = vector.begin() + index;
    vector.insert(position, { name, type });
}

/// <summary>
/// Returns the number of variables of the given kind already defined in the table.
/// </summary>
int SymbolTable::varCount(ESymbolKind kind)
{
    switch (kind)
    {
    case ESymbolKind::STATIC:
        return (int)staticSymbols.size();
    case ESymbolKind::FIELD:
        return (int)fieldSymbols.size();
    case ESymbolKind::ARG:
        return (int)argSymbols.size();
    case ESymbolKind::VAR:
        return (int)varSymbols.size();
    case ESymbolKind::UNDEFINED:
    default:
        return -1;
    }
}

/// <summary>
/// Retrieves information about a symbol with the given name from the symbol table.
/// If the symbol is not found, returns a Symbol with default values.
/// </summary>
/// <param name="name">The name of the symbol to retrieve.</param>
/// <returns>A Symbol struct containing information about the symbol.</returns>
Symbol SymbolTable::getSymbol(std::string name)
{
    int index = -1;

    index = findSymbol(varSymbols, name);
    if (index != -1)
    {
        return Symbol
        {
            name,
            ESymbolKind::VAR,
            varSymbols[index].second,
            index
        };
    }

    index = findSymbol(argSymbols, name);
    if (index != -1)
    {
        return Symbol
        {
            name,
            ESymbolKind::ARG,
            argSymbols[index].second,
            index
        };
    }

    index = findSymbol(fieldSymbols, name);
    if (index != -1)
    {
        return Symbol
        {
            name,
            ESymbolKind::FIELD,
            fieldSymbols[index].second,
            index
        };
    }

    index = findSymbol(staticSymbols, name);
    if (index != -1)
    {
        return Symbol
        {
            name,
            ESymbolKind::STATIC,
            staticSymbols[index].second,
            index
        };
    }

    return Symbol{};
}

std::vector<std::pair<std::string, std::string>>& SymbolTable::getVector(ESymbolKind symbolKind)
{
    switch (symbolKind)
    {
    case ESymbolKind::STATIC:
        return staticSymbols;
    case ESymbolKind::FIELD:
        return fieldSymbols;
    case ESymbolKind::ARG:
        return argSymbols;
    case ESymbolKind::VAR:
        return varSymbols;
    case ESymbolKind::UNDEFINED:
    default:
        throw JackCompilerError("Cannot define UNDEFINED symbol.");
    }
}

int SymbolTable::findSymbol(const std::vector<std::pair<std::string, std::string>>& symbols, const std::string& key)
{
    for (auto i = 0; i <= symbols.size(); i++)
    {
        if (symbols[i].first == key)
        {
            return i;
        }
    }
    return -1;
}
