#include "VMTranslator.h"

#include <iostream>
#include <filesystem>
#include "Parser.h"

#define SAFE_DELETE(x) if(x) { delete x; x = NULL; }

namespace fs = std::filesystem;

VMTranslator::VMTranslator(std::string path)
{
    fs::path filePath = path;
    fs::path inputPath = filePath.is_relative()
        ? fs::current_path().string() + "\\" + filePath.string() 
        : filePath.string();
    
    this->input_file = inputPath.string();
    this->is_directory_path = inputPath.extension() == "";

    std::string outputPrefix = filePath.is_relative()
        ? fs::current_path().string() + "\\"
        : "";
    std::string outputPath = this->is_directory_path
        ? filePath.string()
        : filePath.parent_path().string();
    std::string outputFileName = this->is_directory_path
        ? filePath.filename().string()
        : filePath.filename().stem().string();

    this->output_file= outputPrefix + outputPath + "\\" + outputFileName + ".asm";

    if (!is_directory_path && !isupper(inputPath.filename().string()[0]))
    {
        throw std::runtime_error("Input file/directory " + std::string(path) + " doesn't start with uppercase.");
    }

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
    if (!fs::exists(filePath))
    {
        throw std::runtime_error("Directory " + input_file + " doesn't exist");
    }

    std::vector<std::string> vmFiles = std::vector<std::string>();
    for (const fs::directory_entry& entry : fs::directory_iterator(input_file))
    {
        if (entry.is_regular_file() && isupper(entry.path().filename().string()[0]) && entry.path().extension() == ".vm")
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
    if (!fs::exists(filePath))
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
        else if (parser.commandType() == ECommandType::C_FUNCTION)
        {
            code_writer->writeFunction(parser.arg1(), parser.arg2());
        }
        else if (parser.commandType() == ECommandType::C_RETURN)
        {
            code_writer->writeReturn();
        }
        else if (parser.commandType() == ECommandType::C_CALL)
        {
            code_writer->writeCall(parser.arg1(), parser.arg2());
        }
    }
}