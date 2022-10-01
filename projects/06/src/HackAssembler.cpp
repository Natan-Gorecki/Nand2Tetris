#include <bitset>
#include <shlwapi.h>
#include <windows.h>
#include "HackAssembler.h"

#pragma comment(lib, "Shlwapi.lib")

#define SAFE_DELETE(x) if(x) { delete x; x = NULL; }

bool isNumber(std::string text)
{
    return !text.empty() && text.find_first_not_of("0123456789") == std::string::npos;
}

/// <summary>
/// Maps symbols from input file (1st run)
/// </summary>
void HackAssembler::searchSymbols()
{
    SAFE_DELETE(parser);
    parser = new Parser(input_file, directory_path);

    
    while (parser->hasMoreLines())
    {
        parser->advance();

        if (parser->instructionType() == InstructionType::L_INSTRUCTION)
        {
            if (std::isdigit(parser->symbol()[0]))
            {
                throw std::runtime_error("Error: label symbol " + parser->symbol() + " starts with digit");
            }

            if (symbol_table->contains(parser->symbol()))
            {
                throw std::runtime_error("Error: label symbol " + parser->symbol() + " is duplicated");
            }

            symbol_table->addEntry(parser->symbol(), parser->lineNumber() + 1);
        }
    }
}

/// <summary>
/// Converts assembler code to machine instructions (2nd run)
/// </summary>
void HackAssembler::assemblerToMachineCode()
{
    SAFE_DELETE(parser);
    parser = new Parser(input_file, directory_path);

    BOOL isRelative = PathIsRelativeA(output_file.c_str());
    std::ofstream* outputStream = NULL;

    if (isRelative)
    {
        outputStream = new std::ofstream(directory_path + output_file);
    }
    else
    {
        outputStream = new std::ofstream(output_file);
    }

    if (!outputStream->is_open())
    {
        throw std::runtime_error("Cannot find or open " + output_file + " file");
    }
    
    int variableAddress = 16;
    while (parser->hasMoreLines())
    {
        parser->advance();

        if (parser->instructionType() == InstructionType::COMMENT
            || parser->instructionType() == InstructionType::L_INSTRUCTION)
        {
            continue;
        }

        if (parser->instructionType() == InstructionType::A_INSTRUCTION)
        {
            int symbolAddress = -1;
            if (std::isdigit(parser->symbol()[0]))
            {
                if (isNumber(parser->symbol()))
                {
                    symbolAddress = std::stoi(parser->symbol());
                }
                else
                {
                    throw std::runtime_error("Error: number " + parser->symbol() + " has invalid format");
                }
            }
            else
            {
                if (!symbol_table->contains(parser->symbol()))
                {
                    symbol_table->addEntry(parser->symbol(), variableAddress++);
                }

                symbolAddress = symbol_table->getAddress(parser->symbol());
                if (symbolAddress == -1)
                {
                    throw std::runtime_error("Error: symbol table doesnt contain " + parser->symbol() + " symbol");
                }
            }

            if (symbolAddress < 0 || symbolAddress > SHRT_MAX)
            {
                throw std::runtime_error("Error: number " + parser->symbol() + " out of short scope");
            }

            std::string binary = std::bitset<16>(symbolAddress).to_string();
            *outputStream << binary << "\n";
            continue;
        }
        else if (parser->instructionType() == InstructionType::C_INSTRUCTION)
        {
            *outputStream
                << "111"
                << CodeModule::comp(parser->comp())
                << CodeModule::dest(parser->dest())
                << CodeModule::jump(parser->jump())
                << "\n";
        }
    }
}

/// <summary>
/// Creates parser, symbol table and handles file paths
/// </summary>
HackAssembler::HackAssembler(std::string inputFile, std::string outputFile)
{
    this->symbol_table = new SymbolTable();

    CHAR buffer[MAX_PATH] = { 0 };
    GetModuleFileNameA(NULL, buffer, MAX_PATH);
    std::string::size_type directorySize = std::string(buffer).find_last_of("\\");
    this->directory_path = std::string(buffer).substr(0, directorySize + 1);

    this->input_file = inputFile;
    this->output_file = outputFile;
}

/// <summary>
/// Releases allocated memory
/// </summary>
HackAssembler::~HackAssembler()
{
    SAFE_DELETE(parser);
    SAFE_DELETE(symbol_table);
}    