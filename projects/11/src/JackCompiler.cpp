#include <filesystem>
#include "CompilationEngine.h"
#include "JackCompiler.h"
#include "JackTokenizer.h"
#include "JackCompilerError.h"

using namespace std;
namespace fs = std::filesystem;

/// <summary>
/// Validates provided path, creates JackTokenizer and CompilationEngine.
/// May throw a runtime exception.
/// </summary>
JackCompiler::JackCompiler(std::string const& path)
{
    fs::path filePath = path;
    fs::path inputPath = filePath.is_relative()
        ? fs::current_path().string() + "\\" + filePath.string()
        : filePath.string();

    mInputPath = inputPath.string();
    mIsDirectoryPath = inputPath.extension() == "";

    if (!mIsDirectoryPath && !isupper(inputPath.filename().string()[0]))
    {
        throw JackCompilerError("Input file " + std::string(path) + " doesn't start with uppercase.");
    }

    if (!mIsDirectoryPath && inputPath.extension() != ".jack")
    {
        throw JackCompilerError("Input file " + std::string(path) + " doesn't have .jack extension");
    }
}

/// <summary>
/// Returns an information, whether provided path is directory or single file.
/// </summary>
bool JackCompiler::isDirectoryPath() const
{
    return mIsDirectoryPath;
}

/// <summary>
/// Parses all .jack files from specified directory.
/// </summary>
void JackCompiler::parseDirectory() const
{
    if (!fs::exists(fs::path(mInputPath)))
    {
        throw JackCompilerError("Directory " + mInputPath + " doesn't exist.");
    }

    auto jackFiles = std::vector<std::string>();
    for (const fs::directory_entry& entry : fs::directory_iterator(mInputPath))
    {
        if (entry.is_regular_file() && isupper(entry.path().filename().string()[0]) && entry.path().extension() == ".jack")
        {
            jackFiles.push_back(entry.path().string());
        }
    }

    if (jackFiles.empty())
    {
        throw JackCompilerError("Directory " + mInputPath + " doesn't contain any file with .jack extension.");
    }

    for (std::string jackFile : jackFiles)
    {
        auto compilationEngine = make_unique<CompilationEngine>(jackFile);
        compilationEngine->compileFile();
    }
}

/// <summary>
/// Parses specified .jack file.
/// </summary>
void JackCompiler::parseSingleFile() const
{
    if (!fs::exists(fs::path(mInputPath)))
    {
        throw JackCompilerError("File " + mInputPath + " doesn't exist.");
    }

    auto compilationEngine = make_unique<CompilationEngine>(mInputPath);
    compilationEngine->compileFile();
}
