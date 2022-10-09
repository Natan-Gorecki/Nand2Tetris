#include <cctype> 
#include <filesystem> //C++17
#include <iostream>

#include "CodeWriter.h"
#include "Parser.h"

int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        std::cout << "Usage: VMTranslator InputFile.vm\n";
        return EXIT_FAILURE;
    }

    try
    {
        std::filesystem::path filePath = argv[1];
        if (isupper(argv[1][0]) == false) 
        {
            throw std::runtime_error("Input file " + std::string(argv[1]) + " doesn't start with uppercase.");
        }
        if (filePath.extension() != ".vm")
        {
            throw std::runtime_error("Input file " + std::string(argv[1]) + " doesn't have .vm extension");
        }

        std::string inputFile, outputFile;
        if (filePath.is_relative())
        {
            inputFile = std::filesystem::current_path().string() + filePath.string();
            outputFile = std::filesystem::current_path().string() + filePath.replace_extension(".asm").string();
        }
        else
        {
            inputFile = filePath.string();
            outputFile = filePath.replace_extension(".asm").string();
        }

        CodeWriter* codeWriter = new CodeWriter(outputFile, true);
        Parser* parser = new Parser(inputFile);

        return EXIT_SUCCESS;
    }
    catch (const std::runtime_error& error)
    {
        std::cout << "Error: " << error.what() << "\n";
        return EXIT_FAILURE;
    }
}