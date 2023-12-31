#include "SymbolTable.h"

using namespace std;

/// <summary>
/// Adds <symbol,address> to the table.
/// </summary>
void SymbolTable::addEntry(const string& symbol, int address)
{
    mSymbolTable.insert_or_assign(symbol, address);
}

/// <summary>
/// Does the symbol table contain the given symbol?
/// </summary>
bool SymbolTable::contains(const string& symbol)
{
    auto it = mSymbolTable.find(symbol);
    return it != mSymbolTable.end();
}

/// <summary>
/// Returns the address associated with the symbol.
/// </summary>
int SymbolTable::getAddress(const string& symbol)
{
    if (auto it = mSymbolTable.find(symbol); it != mSymbolTable.end())
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
    addEntry(string("R0"),   0);
    addEntry(string("R1"),   1);
    addEntry(string("R2"),   2);
    addEntry(string("R3"),   3);
    addEntry(string("R4"),   4);
    addEntry(string("R5"),   5);
    addEntry(string("R6"),   6);
    addEntry(string("R7"),   7);
    addEntry(string("R8"),   8);
    addEntry(string("R9"),   9);
    addEntry(string("R10"), 10);
    addEntry(string("R11"), 11);
    addEntry(string("R12"), 12);
    addEntry(string("R13"), 13);
    addEntry(string("R14"), 14);
    addEntry(string("R15"), 15);

    addEntry(string("SP"),   0);
    addEntry(string("LCL"),  1);
    addEntry(string("ARG"),  2);
    addEntry(string("THIS"), 3);
    addEntry(string("THAT"), 4);

    addEntry(string("SCREEN"), 16384);
    addEntry(string("KBD"),    24576);
}
