#pragma once

#include <string>

#include "CodeModule.h"
#include "Parser.h"
#include "SymbolTable.h"

/// <summary>
/// HackAssembler contains main implementation to map symbols from input file,
///     then to convert assembler code to machine instructions
/// <summary>
class HackAssembler
{
public:
    /// <summary>
    /// Maps symbols from input file (1st run)
    /// </summary>
    void searchSymbols();
    /// <summary>
    /// Converts assembler code to machine instructions (2nd run)
    /// </summary>
    void assemblerToMachineCode(bool allowOverflowError);

    /// <summary>
    /// Creates parser, symbol table and handles file paths
    /// </summary>
    explicit HackAssembler(const std::string& inputFile);

private:
    void writeAInstruction(std::ofstream* outputStream, int& variableAddress, bool allowOverflowError);
    void writeCInstruction(std::ofstream* outputStream);

    std::unique_ptr<Parser> mParser;
    std::unique_ptr<SymbolTable> mSymbolTable = std::make_unique<SymbolTable>();
    std::string mInputFileName;
    std::string mOutputFileName;
};
