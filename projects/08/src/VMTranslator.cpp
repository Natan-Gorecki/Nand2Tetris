#include "VMTranslator.h"

#include <iostream>
#include <filesystem>
#include "CodeWriter.h"
#include "Parser.h"


VMTranslator::VMTranslator(std::string path)
{
    std::filesystem::path filePath = path;

    if (isupper(path[0]) == false)
    {
        throw std::runtime_error("Input file/directory " + std::string(path) + " doesn't start with uppercase.");
    }

    std::filesystem::path inputPath, outputPath;
    if (filePath.is_relative())
    {
        inputPath = std::filesystem::current_path().string() + "\\" + filePath.string();
        outputPath = std::filesystem::current_path().string() + "\\" + filePath.replace_extension(".asm").string();
    }
    else
    {
        inputPath = filePath.string();
        outputPath = filePath.replace_extension(".asm").string();
    }

    this->input_file = inputPath.string();
    this->output_file = outputPath.string();
    this->is_directory_path = inputPath.extension() == "";

    if (!is_directory_path && inputPath.extension() != ".vm")
    {
        throw std::runtime_error("Input file " + std::string(path) + " doesn't have .vm extension");
    }
}

VMTranslator::~VMTranslator()
{
}

bool VMTranslator::isDirectoryPath()
{
    return is_directory_path;
}

void VMTranslator::parseDirectory()
{
    throw std::runtime_error("Not implemented exception");
}

void VMTranslator::parseSingleFile()
{
    std::string outputFile, inputFile;
    throw std::runtime_error("Not implemented exception");

    CodeWriter codeWriter = CodeWriter(outputFile, true);
    Parser parser = Parser(inputFile);

    while (parser.hasMoreLines())
    {
        parser.advance();
        if (parser.commandType() == ECommandType::UNDEFINED)
        {
            throw std::runtime_error("Unknown command.");
        }
        else if (parser.commandType() == ECommandType::COMMENT)
        {
            continue;
        }
        else if (parser.commandType() == ECommandType::C_ARITHMETIC)
        {
            codeWriter.writeArithmetic(parser.arg1());
        }
        else if (parser.commandType() == ECommandType::C_PUSH || parser.commandType() == ECommandType::C_POP)
        {
            codeWriter.writePushPop(parser.commandType(), parser.arg1(), parser.arg2());
        }
    }
}