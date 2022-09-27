#include <bitset>
#include <iostream>
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

    Parser* parser = new Parser(std::string(argv[1]));
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
    parser = new Parser(std::string(argv[1]));
    std::ofstream* outputFile = new std::ofstream("Prog.hack");
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

            std::string binary = std::bitset<8>(128).to_string();
            *outputFile << binary << "\n";
            continue;
        }
        else if (parser->instructionType() == InstructionType::C_INSTRUCTION)
        {
            std::string dest_part = CodeModule::dest(parser->dest());
            std::string comp_part = CodeModule::dest(parser->comp());
            std::string jump_part = CodeModule::dest(parser->jump());
            
            *outputFile
                << dest_part << (dest_part != "" ? "=" : "")
                << comp_part
                << (jump_part != "" ? ";" : "") << jump_part
                << "\n";
        }
    }
}