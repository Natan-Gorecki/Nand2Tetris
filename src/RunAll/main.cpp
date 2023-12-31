#include <filesystem>
#include <functional>
#include <iostream>

namespace fs = std::filesystem;

enum class FileOperation
{
    COPY,
    MOVE
};

fs::path getExecutablePath(char* arg);
fs::path getOSPath(const fs::path& path);
bool isDirectoryPath(char* path);
void ensureDirectoryExist(const fs::path& path);
void moveFiles(const fs::path& inputPath, const fs::path& outputPath, const char* extension, FileOperation fileOperation);
void executeConsoleApplication(const fs::path& path, const fs::path& arg, const char* arg2 = "");

int main(int argc, char* argv[])
{
    if (argc != 3)
    {
        std::cout << "Usage: \n"
            << "\tRunAll InputDirectory OutputDirectory\n"
            << "\tRunAll InputFile.jack OutputDirectory\n";
        return EXIT_FAILURE;
    }

    if (!isDirectoryPath(argv[2]))
    {
        std::cout << "'" << argv[2] << "' is not directory!";
        return EXIT_FAILURE;
    }

    auto inputPath = fs::path(argv[1]);
    auto outputPath = fs::path(argv[2]);

    auto exePath = getExecutablePath(argv[0]);
    auto osPath = getOSPath(exePath);

    ensureDirectoryExist(outputPath);
    ensureDirectoryExist(outputPath / "jack_files");
    ensureDirectoryExist(outputPath / "xml_files");
    ensureDirectoryExist(outputPath / "vm_files");
    ensureDirectoryExist(outputPath / "assembler_files");

    std::cout << "\nCopying source files to output directory...\n";
    moveFiles(osPath, outputPath / "jack_files", ".jack", FileOperation::COPY);

    std::cout << std::endl;
    moveFiles(inputPath, outputPath / "jack_files", ".jack", FileOperation::COPY);

    std::cout << "\nCompiling source files using JackCompiler...\n";
    executeConsoleApplication(exePath / "JackCompiler.exe", outputPath / "jack_files");

    std::cout << "\nMoving created files to suitable directories...\n";
    moveFiles(outputPath / "jack_files", outputPath / "xml_files", ".xml", FileOperation::MOVE);
    
    std::cout << std::endl;
    moveFiles(outputPath / "jack_files", outputPath / "vm_files", ".vm", FileOperation::MOVE);

    std::cout << "\nTranslating VM code to assembly code...\n";
    executeConsoleApplication(exePath / "VMTranslator.exe", outputPath / "vm_files");
    std::cout << "Moving assembly file to suitable directory...\n";
    fs::rename(outputPath / "vm_files" / "vm_files.asm", outputPath / "assembler_files" / fs::path(inputPath.stem().string() + ".asm"));

    std::cout << "\nTranslating assembly code to machine code...\n";
    executeConsoleApplication(exePath / "HackAssembler.exe", outputPath / "assembler_files" / fs::path(inputPath.stem().string() + ".asm"), "--alowOverflowError");
}

fs::path getExecutablePath(char* arg)
{
    return fs::weakly_canonical(fs::path(arg)).parent_path();
}

fs::path getOSPath(const fs::path& exePath)
{
    fs::path path(exePath);

    while (!path.empty() && path.filename() != "Nand2Tetris")
    {
        path = path.parent_path();
    }

    if (path.empty())
    {
        std::cout << "Could not find directory with OS implementation! Aborting...\n";
        exit(EXIT_FAILURE);
    }

    fs::path osPath = path / "projects" / "12";
    std::cout << "Found OS path: " << osPath << ".\n";
    return osPath;
}

bool isDirectoryPath(char* path)
{
    auto fs_path = fs::path(path);
    return fs_path.extension() == "";
}

void ensureDirectoryExist(const fs::path& path)
{
    if (!fs::exists(path))
    {
        std::cout << "Creating " << path << " directory...\n";
        fs::create_directory(path);
        return;
    }
    std::cout << "Directory " << path << " exist.\n";
}

void moveFiles(const fs::path& inputPath, const fs::path& outputPath, const char* extension, FileOperation fileOperation)
{
    std::function<void(const fs::path&, const fs::path&)> copyFile = [](const fs::path& input, const fs::path& output)
        {
            fs::copy_file(input, output, fs::copy_options::overwrite_existing);
        };
    std::function<void(const fs::path&, const fs::path&)> moveFile = [](const fs::path& input, const fs::path& output)
        {
            fs::rename(input, output);
        };
    auto operationString = fileOperation == FileOperation::COPY ? "Copying " : "Moving ";
    const auto& performFileOperation = fileOperation == FileOperation::COPY ? copyFile : moveFile;

    if (!fs::exists(inputPath))
    {
        std::cerr << "Input path or directory " << inputPath << " does not exist! Aborting...";
        exit(EXIT_FAILURE);
    }

    if (fs::is_regular_file(inputPath))
    {
        if (inputPath.extension() != extension)
        {
            std::cout << "File " << inputPath << " extension is not '" << extension << "'! Aborting...\n";
            exit(EXIT_FAILURE);
        }

        std::cout << operationString << inputPath << "...\n";
        performFileOperation(inputPath, outputPath / inputPath.filename());
        return;
    }

    if (auto iterator = fs::directory_iterator(inputPath); iterator == fs::end(iterator))
    {
        std::cout << "Input directory " << inputPath << " is empty. Aborting...\n";
        exit(EXIT_FAILURE);
    }

    for (const auto& entry : fs::directory_iterator(inputPath))
    {
        if (fs::is_regular_file(entry) && entry.path().extension() == extension)
        {
            std::cout << operationString << entry << "...\n";
            performFileOperation(entry.path(), outputPath / entry.path().filename());
        }
    }
}

void executeConsoleApplication(const fs::path& path, const fs::path& arg, const char* arg2)
{
    std::string command = path.string() + " " + arg.string() + " " + arg2;
    auto result = system(command.c_str());

    if (result != 0)
    {
        std::cout << "Failed to execute command! Aborting...\n";
        exit(EXIT_FAILURE);
    }
}