#include <iostream>
#include <filesystem>
#include "Parser.h"
#include "VMTranslator.h"
#include "VMTranslatorError.h"

using namespace std;
namespace fs = std::filesystem;

VMTranslator::VMTranslator(const string& path)
{
    fs::path filePath = path;
    fs::path inputPath = filePath.is_relative()
        ? fs::current_path().string() + "\\" + filePath.string() 
        : filePath.string();
    
    mInputFileName = inputPath.string();
    mIsDirectoryPath = inputPath.extension() == "";

    string outputPrefix = filePath.is_relative()
        ? fs::current_path().string() + "\\"
        : "";
    string outputPath = mIsDirectoryPath
        ? filePath.string()
        : filePath.parent_path().string();
    string outputFileName = mIsDirectoryPath
        ? filePath.filename().string()
        : filePath.filename().stem().string();

    mOutputFileName = outputPrefix + outputPath + "\\" + outputFileName + ".asm";

    if (!mIsDirectoryPath && !isupper(inputPath.filename().string()[0]))
    {
        throw VMTranslatorError("Input file/directory " + string(path) + " doesn't start with uppercase.");
    }

    if (!mIsDirectoryPath && inputPath.extension() != ".vm")
    {
        throw VMTranslatorError("Input file " + string(path) + " doesn't have .vm extension");
    }

    mCodeWriter = make_unique<CodeWriter>(mOutputFileName, true);
}

bool VMTranslator::isDirectoryPath() const
{
    return mIsDirectoryPath;
}

void VMTranslator::parseDirectory()
{
    if (auto filePath = fs::path(mInputFileName); !fs::exists(filePath))
    {
        throw VMTranslatorError("Directory " + mInputFileName + " doesn't exist");
    }

    auto vmFiles = vector<string>();
    for (const auto& entry : fs::directory_iterator(mInputFileName))
    {
        if (entry.is_regular_file() && isupper(entry.path().filename().string()[0]) && entry.path().extension() == ".vm")
        {
            vmFiles.push_back(entry.path().string());
        }
    }
    
    if (vmFiles.empty())
    {
        throw VMTranslatorError("Directory " + mInputFileName + " doesn't contain any .vm extension");
    }

    for (const auto& vmFile : vmFiles)
    {
        parseSingleFile(vmFile);
    }

    mCodeWriter->validateGotoStatements();
}

void VMTranslator::parseSingleFile()
{
    if (auto filePath = fs::path(mInputFileName); !fs::exists(filePath))
    {
        throw VMTranslatorError("File " + mInputFileName + " doesn't exist");
    }

    parseSingleFile(mInputFileName);

    mCodeWriter->validateGotoStatements();
}

string VMTranslator::getOutputFile() const
{
    return mOutputFileName;
}

void VMTranslator::parseSingleFile(const string& path)
{
    auto parser = Parser(path);

    auto fileName = fs::path(path).replace_extension("").filename().string();
    mCodeWriter->setFileName(fileName);

    while (parser.hasMoreLines())
    {
        parser.advance();
        if (parser.commandType() == ECommandType::UNDEFINED)
        {
            throw VMTranslatorError("Unknown command.");
        }
        else if (parser.commandType() == ECommandType::COMMENT)
        {
            continue;
        }
        else if (parser.commandType() == ECommandType::C_ARITHMETIC)
        {
            mCodeWriter->writeArithmetic(parser.arg1());
        }
        else if (parser.commandType() == ECommandType::C_PUSH || parser.commandType() == ECommandType::C_POP)
        {
            mCodeWriter->writePushPop(parser.commandType(), parser.arg1(), parser.arg2());
        }
        else if (parser.commandType() == ECommandType::C_LABEL)
        {
            mCodeWriter->writeLabel(parser.arg1());
        }
        else if (parser.commandType() == ECommandType::C_GOTO)
        {
            mCodeWriter->writeGoto(parser.arg1());
        }
        else if (parser.commandType() == ECommandType::C_IF)
        {
            mCodeWriter->writeIf(parser.arg1());
        }
        else if (parser.commandType() == ECommandType::C_FUNCTION)
        {
            mCodeWriter->writeFunction(parser.arg1(), parser.arg2());
        }
        else if (parser.commandType() == ECommandType::C_RETURN)
        {
            mCodeWriter->writeReturn();
        }
        else if (parser.commandType() == ECommandType::C_CALL)
        {
            mCodeWriter->writeCall(parser.arg1(), parser.arg2());
        }
    }
}
