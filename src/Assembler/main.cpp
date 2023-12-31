#include <chrono>
#include <iostream>
#include "HackAssembler.h"
#include "HackAssemblerError.h"

int main(int argc, char* argv[])
{
    bool allowOverflowError = false;

    if (argc == 3 && std::string(argv[2]) != "--alowOverflowError")
    {
        std::cout << "Usage: HackAssembler InputFile.asm --alowOverflowError\n";
        return EXIT_FAILURE;
    }

    if (argc == 3 && std::string(argv[2]) == "--alowOverflowError")
    {
        allowOverflowError = true;
    }

    if (argc != 2 && argc != 3)
    {
        std::cout << "Usage: HackAssembler InputFile.asm --alowOverflowError\n";
        return EXIT_FAILURE;
    }

    try
    {
        auto hackAssembler = HackAssembler(argv[1]);
        
        auto startTime = std::chrono::high_resolution_clock::now();
        hackAssembler.searchSymbols();
        auto endTime = std::chrono::high_resolution_clock::now();
        std::cout << "Label symbols searched in " << (endTime - startTime) / std::chrono::milliseconds(1) << " ms.\n";

        startTime = std::chrono::high_resolution_clock::now();
        hackAssembler.assemblerToMachineCode(allowOverflowError);
        endTime = std::chrono::high_resolution_clock::now();
        std::cout << "Machine code created in " << (endTime - startTime) / std::chrono::milliseconds(1) << " ms.\n";

        return EXIT_SUCCESS;
    }
    catch (const HackAssemblerError& error)
    {
        std::cout << error.what() << "\n";
        return EXIT_FAILURE;
    }
}
