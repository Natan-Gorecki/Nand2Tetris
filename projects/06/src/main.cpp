#include <bitset>
#include <iostream>
#include <string>
#include <windows.h>

#include "CodeModule.h"
#include "Parser.h"
#include "SymbolTable.h"

bool isNumber(std::string text)
{
    return !text.empty() && text.find_first_not_of("0123456789") == std::string::npos;
}

int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        std::cout << "Usage: HackAssembler Prog.asm\n";
        return EXIT_FAILURE;
    }

    CHAR buffer[MAX_PATH] = { 0 };
    GetModuleFileNameA(NULL, buffer, MAX_PATH);
    std::string::size_type directorySize = std::string(buffer).find_last_of("\\");
    std::string directoryPath = std::string(buffer).substr(0, directorySize + 1);

    Parser* parser = new Parser(directoryPath + std::string(argv[1]));
    SymbolTable* symbolTable = new SymbolTable();

    // map symbols
    int variableAddress = 16;
    while (parser->hasMoreLines())
    {
        parser->advance();

        if (parser->instructionType() == InstructionType::COMMENT
            || parser->instructionType() == InstructionType::C_INSTRUCTION)
        {
            continue;
        }

        if (parser->instructionType() == InstructionType::L_INSTRUCTION)
        {
            if (symbolTable->contains(parser->symbol()))
            {
                std::cout << "Error: label symbol " + parser->symbol() + " is already declared";
                return EXIT_FAILURE;
            }

            symbolTable->addEntry(parser->symbol(), parser->lineNumber() + 1);
        }
        else if (parser->instructionType() == InstructionType::A_INSTRUCTION)
        {
            if (isNumber(parser->symbol()))
            {
                continue;
            }

            if (!symbolTable->contains(parser->symbol()))
            {
                symbolTable->addEntry(parser->symbol(), variableAddress++);
            }
        }
    }

    // convert assembler instruction to machine code
    delete parser;
    parser = new Parser(directoryPath + std::string(argv[1]));
    std::ofstream* outputFile = new std::ofstream(directoryPath + "Prog.hack");
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
            if (isNumber(parser->symbol()))
            {
                symbolAddress = std::stoi(parser->symbol());   
            }
            else
            {
                symbolAddress = symbolTable->getAddress(parser->symbol());
                if (symbolAddress == -1)
                {
                    std::cout << "Error: symbol table doesnt contain " + parser->symbol() + " symbol";
                    return EXIT_FAILURE;
                }
            }

            if (symbolAddress < 0 || symbolAddress > SHRT_MAX)
            {
                std::cout << "Error: number " + parser->symbol() + " out of short scope";
                return EXIT_FAILURE;
            }

            std::string binary = std::bitset<16>(symbolAddress).to_string();
            *outputFile << binary << "\n";
            continue;
        }
        else if (parser->instructionType() == InstructionType::C_INSTRUCTION)
        {
            *outputFile
                << "111"
                << CodeModule::comp(parser->comp())
                << CodeModule::dest(parser->dest())
                << CodeModule::jump(parser->jump())
                << "\n";
        }
    }
}