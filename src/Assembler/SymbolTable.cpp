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
    return -1;
}

/// <summary>
/// Creates a new empty symbol table.
/// </summary>
SymbolTable::SymbolTable()
{
    addEntry(std::string("R0"),   0);
    addEntry(std::string("R1"),   1);
    addEntry(std::string("R2"),   2);
    addEntry(std::string("R3"),   3);
    addEntry(std::string("R4"),   4);
    addEntry(std::string("R5"),   5);
    addEntry(std::string("R6"),   6);
    addEntry(std::string("R7"),   7);
    addEntry(std::string("R8"),   8);
    addEntry(std::string("R9"),   9);
    addEntry(std::string("R10"), 10);
    addEntry(std::string("R11"), 11);
    addEntry(std::string("R12"), 12);
    addEntry(std::string("R13"), 13);
    addEntry(std::string("R14"), 14);
    addEntry(std::string("R15"), 15);

    addEntry(std::string("SP"),   0);
    addEntry(std::string("LCL"),  1);
    addEntry(std::string("ARG"),  2);
    addEntry(std::string("THIS"), 3);
    addEntry(std::string("THAT"), 4);

    addEntry(std::string("SCREEN"), 16384);
    addEntry(std::string("KBD"),    24576);
}
