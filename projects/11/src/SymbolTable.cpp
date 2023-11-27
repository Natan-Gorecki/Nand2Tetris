#include "JackCompilerError.h"
#include "SymbolTable.h"

/// <summary>
/// Defines (adds to the table) a new variable of the given name, type, and kind.
/// Assings to it the index value of that kind, and adds 1 to the index.
/// </summary>
void SymbolTable::define(std::string name, std::string type, ESymbolKind kind)
{
    switch (kind)
    {
    case ESymbolKind::STATIC:
        staticSymbols.push_back({ name, type });
        break;
    case ESymbolKind::FIELD:
        fieldSymbols.push_back({ name, type });
        break;
    case ESymbolKind::ARG:
        argSymbols.push_back({ name, type });
        break;
    case ESymbolKind::VAR:
        varSymbols.push_back({ name, type });
        break;
    case ESymbolKind::UNDEFINED:
    default:
        throw JackCompilerError("Cannot define UNDEFINED symbol.");
    }
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
/// Returns the kind of the named identifier.
/// If the identifier is not found, returns UNDEFINED.
/// </summary>
ESymbolKind SymbolTable::kindOf(std::string name)
{
    int index = -1;

    index = findSymbol(varSymbols, name);
    if (index != -1)
    {
        return ESymbolKind::VAR;
    }

    index = findSymbol(argSymbols, name);
    if (index != -1)
    {
        return ESymbolKind::ARG;
    }

    index = findSymbol(fieldSymbols, name);
    if (index != -1)
    {
        return ESymbolKind::FIELD;
    }

    index = findSymbol(staticSymbols, name);
    if (index != -1)
    {
        return ESymbolKind::STATIC;
    }

    return ESymbolKind::UNDEFINED;
}

/// <summary>
/// Returns the type of the named variable.
/// </summary>
std::string SymbolTable::typeOf(std::string name)
{
    int index = -1;

    index = findSymbol(varSymbols, name);
    if (index != -1)
    {
        return varSymbols[index].second;
    }

    index = findSymbol(argSymbols, name);
    if (index != -1)
    {
        return argSymbols[index].second;
    }

    index = findSymbol(fieldSymbols, name);
    if (index != -1)
    {
        return fieldSymbols[index].second;
    }

    index = findSymbol(staticSymbols, name);
    if (index != -1)
    {
        return staticSymbols[index].second;
    }

    return "";
}

/// <summary>
/// Returns the index of the named variable.
/// </summary>
int SymbolTable::indexOf(std::string name)
{
    int index = -1;

    index = findSymbol(varSymbols, name);
    if (index != -1)
    {
        return index;
    }

    index = findSymbol(argSymbols, name);
    if (index != -1)
    {
        return index;
    }

    index = findSymbol(fieldSymbols, name);
    if (index != -1)
    {
        return index;
    }

    index = findSymbol(staticSymbols, name);
    return index;
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
