#include <filesystem>
#include "JackAnalyzer.h"

namespace fs = std::filesystem;

/// <summary>
/// Validates provided path, creates JackTokenizer and CompilationEngine.
/// May throw a runtime exception.
/// </summary>
JackAnalyzer::JackAnalyzer(std::string path)
{
    fs::path filePath = path;
    fs::path inputPath = filePath.is_relative()
        ? fs::current_path().string() + "\\" + filePath.string()
        : filePath.string();

    mInputFile = inputPath.string();
    mIsDirectoryPath = inputPath.extension() == "";

    std::string outputPrefix = filePath.is_relative()
        ? fs::current_path().string() + "\\"
        : "";
    std::string outputPath = mIsDirectoryPath
        ? filePath.string()
        : filePath.parent_path().string();
    std::string outputFileName = mIsDirectoryPath
        ? filePath.filename().string()
        : filePath.filename().stem().string();

    mOutputFile = outputPrefix + outputPath + "\\" + outputFileName + "XT" + ".xml";

    if (!mIsDirectoryPath && !isupper(inputPath.filename().string()[0]))
    {
        throw std::runtime_error("Input file " + std::string(path) + " doesn't start with uppercase.");
    }

    if (!mIsDirectoryPath && inputPath.extension() != ".jack")
    {
        throw std::runtime_error("Input file " + std::string(path) + " doesn't have .jack extension");
    }
}

/// <summary>
/// Releases allocated memory.
/// </summary>
JackAnalyzer::~JackAnalyzer()
{

}

/// <summary>
/// Returns an information, whether provided path is directory or single file.
/// </summary>
bool JackAnalyzer::isDirectoryPath()
{
    return mIsDirectoryPath;
}

/// <summary>
/// Parses all .jack files from specified directory.
/// </summary>
void JackAnalyzer::parseDirectory()
{
    fs::path filePath = fs::path(mInputFile);
    if (!fs::exists(filePath))
    {
        throw std::runtime_error("Directory " + mInputFile + " doesn't exist.");
    }

    std::vector<std::string> jackFiles = std::vector<std::string>();
    for (const fs::directory_entry& entry : fs::directory_iterator(mInputFile))
    {
        if (entry.is_regular_file() && isupper(entry.path().filename().string()[0]) && entry.path().extension() == ".jack")
        {
            jackFiles.push_back(entry.path().string());
        }
    }

    if (jackFiles.size() == 0)
    {
        throw std::runtime_error("Directory " + mInputFile + " doesn't contain any file with .jack extension.");
    }

    for (std::string jackFile : jackFiles)
    {
        parseSingleFile(jackFile);
    }
}

/// <summary>
/// Parses specified .jack file.
/// </summary>
void JackAnalyzer::parseSingleFile()
{
    fs::path filePath = fs::path(mInputFile);
    if (!fs::exists(filePath))
    {
        throw std::runtime_error("File " + mInputFile + " doesn't exist.");
    }

    parseSingleFile(mInputFile);
}

std::string JackAnalyzer::getOutputFile()
{
    return this->mOutputFile;
}

void JackAnalyzer::parseSingleFile(std::string path)
{

}
