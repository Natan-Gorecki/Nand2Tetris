#include <iostream>

int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        std::cout << "Usage: VMTranslator InputFile.vm\n";
        return EXIT_FAILURE;
    }

    try
    {
        return EXIT_SUCCESS;
    }
    catch (const std::runtime_error& error)
    {
        std::cout << error.what() << "\n";
        return EXIT_FAILURE;
    }
}