#include <filesystem>
#include <iostream>

int main(int argc, char* argv[])
{
    if (argc > 2)
    {
        std::cout << "Usage: \n"
            << "\RunAll InputDirectory\n"
            << "\tRunAll InputFile.jack\n";
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
}