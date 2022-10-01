#include <chrono>
#include <iostream>
#include "HackAssembler.h"

int main(int argc, char* argv[])
{
    if (argc != 3)
    {
        std::cout << "Usage: HackAssembler InputFile.asm OutputFile.hack\n";
        return EXIT_FAILURE;
    }

    try
    {
        HackAssembler hackAssembler = HackAssembler(argv[1], argv[2]);
        
        auto startTime = std::chrono::high_resolution_clock::now();
        hackAssembler.searchSymbols();
        auto endTime = std::chrono::high_resolution_clock::now();
        std::cout << "Label symbols searched in " << (endTime - startTime) / std::chrono::milliseconds(1) << " ms.\n";

        startTime = std::chrono::high_resolution_clock::now();
        hackAssembler.assemblerToMachineCode();
        endTime = std::chrono::high_resolution_clock::now();
        std::cout << "Machine code created in " << (endTime - startTime) / std::chrono::milliseconds(1) << " ms.\n";

        return EXIT_SUCCESS;
    }
    catch (const std::runtime_error& error)
    {
        std::cout << error.what() << "\n";
        return EXIT_FAILURE;
    }
}