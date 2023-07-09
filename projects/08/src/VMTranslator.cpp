#include "VMTranslator.h"

#include <iostream>
#include <filesystem>
#include "Parser.h"

#define SAFE_DELETE(x) if(x) { delete x; x = NULL; }

namespace fs = std::filesystem;

VMTranslator::VMTranslator(std::string path)
{
    fs::path filePath = path;

    if (isupper(path[0]) == false)
    {
        throw std::runtime_error("Input file/directory " + std::string(path) + " doesn't start with uppercase.");
    }

    fs::path inputPath, outputPath;
    if (filePath.is_relative())
    {
        inputPath = fs::current_path().string() + "\\" + filePath.string();
        outputPath = fs::current_path().string() + "\\" + filePath.replace_extension(".asm").string();
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

    code_writer = new CodeWriter(output_file, true);
}

VMTranslator::~VMTranslator()
{
    SAFE_DELETE(code_writer);
}

bool VMTranslator::isDirectoryPath()
{
    return is_directory_path;
}

void VMTranslator::parseDirectory()
{
    fs::path filePath = fs::path(input_file);
    if (fs::exists(filePath) == false)
    {
        throw std::runtime_error("Directory " + input_file + " doesn't exist");
    }

    std::vector<std::string> vmFiles = std::vector<std::string>();
    for (const fs::directory_entry& entry : fs::directory_iterator(input_file))
    {
        if (entry.is_regular_file() && entry.path().extension() == ".vm")
        {
            vmFiles.push_back(entry.path().string());
        }
    }
    
    if (vmFiles.size() == 0)
    {
        throw std::runtime_error("Directory " + input_file + " doesn't contain any .vm extension");
    }

    for (std::string vmFile : vmFiles)
    {
        parseSingleFile(vmFile);
    }

    code_writer->validateGotoStatements();
}

void VMTranslator::parseSingleFile()
{
    fs::path filePath = fs::path(input_file);
    if (fs::exists(filePath) == false)
    {
        throw std::runtime_error("File " + input_file + " doesn't exist");
    }
    
    parseSingleFile(input_file);

    code_writer->validateGotoStatements();
}

std::string VMTranslator::getOutputFile()
{
    return this->output_file;
}

void VMTranslator::parseSingleFile(std::string path)
{
    Parser parser = Parser(path);

    std::string fileName = fs::path(path).replace_extension("").filename().string();
    code_writer->setFileName(fileName);

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
            code_writer->writeArithmetic(parser.arg1());
        }
        else if (parser.commandType() == ECommandType::C_PUSH || parser.commandType() == ECommandType::C_POP)
        {
            code_writer->writePushPop(parser.commandType(), parser.arg1(), parser.arg2());
        }
        else if (parser.commandType() == ECommandType::C_LABEL)
        {
            code_writer->writeLabel(parser.arg1());
        }
        else if (parser.commandType() == ECommandType::C_GOTO)
        {
            code_writer->writeGoto(parser.arg1());
        }
        else if (parser.commandType() == ECommandType::C_IF)
        {
            code_writer->writeIf(parser.arg1());
        }
    }
}