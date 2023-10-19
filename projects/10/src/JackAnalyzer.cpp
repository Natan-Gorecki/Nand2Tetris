#include <filesystem>
#include "CompilationEngine.h"
#include "JackAnalyzer.h"
#include "JackTokenizer.h"

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

    mInputPath = inputPath.string();
    mIsDirectoryPath = inputPath.extension() == "";

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
    fs::path filePath = fs::path(mInputPath);
    if (!fs::exists(filePath))
    {
        throw std::runtime_error("Directory " + mInputPath + " doesn't exist.");
    }

    std::vector<std::string> jackFiles = std::vector<std::string>();
    for (const fs::directory_entry& entry : fs::directory_iterator(mInputPath))
    {
        if (entry.is_regular_file() && isupper(entry.path().filename().string()[0]) && entry.path().extension() == ".jack")
        {
            jackFiles.push_back(entry.path().string());
        }
    }

    if (jackFiles.size() == 0)
    {
        throw std::runtime_error("Directory " + mInputPath + " doesn't contain any file with .jack extension.");
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
    fs::path filePath = fs::path(mInputPath);
    if (!fs::exists(filePath))
    {
        throw std::runtime_error("File " + mInputPath + " doesn't exist.");
    }

    parseSingleFile(mInputPath);
}

void JackAnalyzer::parseSingleFile(std::string path)
{
    fs::path outputPath = path;
    std::string filename = outputPath.filename().stem().string();
    std::string outputFile = outputPath.replace_filename(filename + "_VS.xml").string();

    JackTokenizer* jackTokenizer = new JackTokenizer(path);
    CompilationEngine* compilationEngine = new CompilationEngine(outputFile, jackTokenizer);

    compilationEngine->compileClass();
    
    /*if (jackTokenizer->hasMoreTokens())
    {
        throw std::runtime_error("JackTokenizer has more tokens. Only one class per file is allowed.");
    }*/

    delete compilationEngine;
    delete jackTokenizer;
}
