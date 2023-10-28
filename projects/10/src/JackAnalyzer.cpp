#include <filesystem>
#include "CompilationEngine.h"
#include "JackAnalyzer.h"
#include "JackTokenizer.h"
#include "JackAnalyzerError.h"

using namespace std;
namespace fs = std::filesystem;

/// <summary>
/// Validates provided path, creates JackTokenizer and CompilationEngine.
/// May throw a runtime exception.
/// </summary>
JackAnalyzer::JackAnalyzer(std::string const& path)
{
    fs::path filePath = path;
    fs::path inputPath = filePath.is_relative()
        ? fs::current_path().string() + "\\" + filePath.string()
        : filePath.string();

    mInputPath = inputPath.string();
    mIsDirectoryPath = inputPath.extension() == "";

    if (!mIsDirectoryPath && !isupper(inputPath.filename().string()[0]))
    {
        throw JackAnalyzerError("Input file " + std::string(path) + " doesn't start with uppercase.");
    }

    if (!mIsDirectoryPath && inputPath.extension() != ".jack")
    {
        throw JackAnalyzerError("Input file " + std::string(path) + " doesn't have .jack extension");
    }
}

/// <summary>
/// Returns an information, whether provided path is directory or single file.
/// </summary>
bool JackAnalyzer::isDirectoryPath() const
{
    return mIsDirectoryPath;
}

/// <summary>
/// Parses all .jack files from specified directory.
/// </summary>
void JackAnalyzer::parseDirectory() const
{
    if (!fs::exists(fs::path(mInputPath)))
    {
        throw JackAnalyzerError("Directory " + mInputPath + " doesn't exist.");
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
        throw JackAnalyzerError("Directory " + mInputPath + " doesn't contain any file with .jack extension.");
    }

    for (std::string jackFile : jackFiles)
    {
        parseSingleFile(jackFile);
    }
}

/// <summary>
/// Parses specified .jack file.
/// </summary>
void JackAnalyzer::parseSingleFile() const
{
    if (!fs::exists(fs::path(mInputPath)))
    {
        throw JackAnalyzerError("File " + mInputPath + " doesn't exist.");
    }

    parseSingleFile(mInputPath);
}

void JackAnalyzer::parseSingleFile(std::string path) const
{
    fs::path outputPath = path;
    std::string filename = outputPath.filename().stem().string();
    std::string outputFile = outputPath.replace_filename(filename + "_New.xml").string();

    auto jackTokenizer = make_shared<JackTokenizer>(path);
    auto compilationEngine = make_unique<CompilationEngine>(outputFile, jackTokenizer);

    compilationEngine->compileClass();

    if (jackTokenizer->advance())
    {
        throw JackAnalyzerError("JackTokenizer has more tokens. Only one class per file is allowed.");
    }
}
