#include <bitset>
#include <filesystem>
#include "HackAssembler.h"
#include "HackAssemblerError.h"

using namespace std;
namespace fs = std::filesystem;

bool isNumber(string_view text)
{
    return !text.empty() && text.find_first_not_of("0123456789") == string::npos;
}

/// <summary>
/// Maps symbols from input file (1st run)
/// </summary>
void HackAssembler::searchSymbols()
{
    mParser = make_unique<Parser>(mInputFileName);

    while (mParser->hasMoreLines())
    {
        mParser->advance();

        if (mParser->instructionType() != InstructionType::L_INSTRUCTION)
        {
            continue;
        }

        if (std::isdigit(mParser->symbol()[0]))
        {
            throw HackAssemblerError("Error: label symbol " + mParser->symbol() + " starts with digit");
        }

        if (mSymbolTable->contains(mParser->symbol()))
        {
            throw HackAssemblerError("Error: label symbol " + mParser->symbol() + " is duplicated");
        }

        mSymbolTable->addEntry(mParser->symbol(), mParser->lineNumber() + 1);
    }
}

/// <summary>
/// Converts assembler code to machine instructions (2nd run)
/// </summary>
void HackAssembler::assemblerToMachineCode(bool allowOverflowError)
{
    mParser = make_unique<Parser>(mInputFileName);

    auto outputStream = make_unique<ofstream>(mOutputFileName);
    if (!outputStream->is_open())
    {
        throw HackAssemblerError("Cannot find or open " + mOutputFileName + " file");
    }
    
    int variableAddress = 16;
    while (mParser->hasMoreLines())
    {
        mParser->advance();

        if (mParser->instructionType() == InstructionType::COMMENT
            || mParser->instructionType() == InstructionType::L_INSTRUCTION)
        {
            continue;
        }

        if (mParser->instructionType() == InstructionType::A_INSTRUCTION)
        {
            writeAInstruction(outputStream.get(), variableAddress, allowOverflowError);
        }
        else if (mParser->instructionType() == InstructionType::C_INSTRUCTION)
        {
            writeCInstruction(outputStream.get());
        }
    }
}

/// <summary>
/// Creates parser, symbol table and handles file paths
/// </summary>
HackAssembler::HackAssembler(const string& inputFile)
{
    fs::path filePath = inputFile;
    if (filePath.extension() != ".asm")
    {
        throw HackAssemblerError("Error: File " + inputFile + " doesn't have .asm extension");
    }

    if (filePath.is_relative()) 
    {
        mInputFileName = fs::current_path().string() + "\\" + filePath.string();
        mOutputFileName = fs::current_path().string() + "\\" + filePath.replace_extension(".hack").string();
    }
    else
    {
        mInputFileName = filePath.string();
        mOutputFileName = filePath.replace_extension(".hack").string();
    }
}

void HackAssembler::writeAInstruction(ofstream* outputStream, int& variableAddress, bool allowOverflowError)
{
    int symbolAddress = -1;
    if (isdigit(mParser->symbol()[0]))
    {
        if (isNumber(mParser->symbol()))
        {
            symbolAddress = stoi(mParser->symbol());
        }
        else
        {
            throw HackAssemblerError("Error: number " + mParser->symbol() + " has invalid format");
        }
    }
    else
    {
        if (!mSymbolTable->contains(mParser->symbol()))
        {
            mSymbolTable->addEntry(mParser->symbol(), variableAddress);
            variableAddress++;
        }

        symbolAddress = mSymbolTable->getAddress(mParser->symbol());
        if (symbolAddress == -1)
        {
            throw HackAssemblerError("Error: symbol table doesnt contain " + mParser->symbol() + " symbol");
        }
    }

    if (symbolAddress < 0 || symbolAddress > SHRT_MAX)
    {
        if (!allowOverflowError)
        {
            throw HackAssemblerError("Error: number " + mParser->symbol() + " out of short scope");
        }
        *outputStream << "OverflowError: @" + to_string(symbolAddress) << "\n";
        return;
    }

    string binary = bitset<16>(symbolAddress).to_string();
    *outputStream << binary << "\n";
}

void HackAssembler::writeCInstruction(ofstream* outputStream)
{
    *outputStream
        << "111"
        << CodeModule::comp(mParser->comp())
        << CodeModule::dest(mParser->dest())
        << CodeModule::jump(mParser->jump())
        << "\n";
}
