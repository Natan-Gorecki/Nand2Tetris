#include <chrono>
#include <iostream>
#include "VMTranslator.h"

int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        std::cout << "Usage: \n"
            << "\tVMTranslator InputDirectory\n"
            << "\tVMTranslator InputFile.vm\n";
        return EXIT_FAILURE;
    }
    
    try
    {
        VMTranslator vmTranslator = VMTranslator(argv[1]);

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
        
        std::cout << "Created assembly code " << argv[1] << ".asm file in " << (endTime - startTime) / std::chrono::milliseconds(1) << " ms.\n";
        return EXIT_SUCCESS;
    }
    catch (const std::runtime_error& error)
    {
        std::cout << "Error: " << error.what() << "\n";
        return EXIT_FAILURE;
    }
}