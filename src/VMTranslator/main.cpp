#include <chrono>
#include <filesystem>
#include <iostream>
#include "VMTranslator.h"
#include "VMTranslatorError.h"

int main(int argc, char* argv[])
{
    if (argc > 2)
    {
        std::cout << "Usage: \n"
            << "\tVMTranslator InputDirectory\n"
            << "\tVMTranslator InputFile.vm\n";
        return EXIT_FAILURE;
    }

    std::string path;
    if (argc == 1)
    {
        std::cout << "No file or directory specifed. Searching for files in current directory.\n";
        path = std::filesystem::current_path().string();
    }
    else
    {
        path = argv[1];
    }
    
    try
    {
        auto vmTranslator = VMTranslator(path);

        auto startTime = std::chrono::high_resolution_clock::now();

        if (vmTranslator.isDirectoryPath())
        {
            vmTranslator.parseDirectory();
        }
        else
        {
            vmTranslator.parseSingleFile();
        }

        auto endTime = std::chrono::high_resolution_clock::now();
        
        std::cout << "Created assembly code " << vmTranslator.getOutputFile() << " file in " << (endTime - startTime) / std::chrono::milliseconds(1) << " ms.\n";
        return EXIT_SUCCESS;
    }
    catch (const VMTranslatorError& error)
    {
        std::cout << "Error: " << error.what() << "\n";
        return EXIT_FAILURE;
    }
}