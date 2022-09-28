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
        hackAssembler.searchSymbols();
        hackAssembler.assemblerToMachineCode();
        return EXIT_SUCCESS;
    }
    catch (const std::runtime_error& error)
    {
        std::cout << error.what() << "\n";
        return EXIT_FAILURE;
    }
}