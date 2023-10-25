#include <filesystem>
#include <functional>
#include <iostream>
#include "Assert.h"
#include "CompilationEngine.h"
#include "Rules/ProgramStructureRules.h"

namespace fs = std::filesystem;

std::function<void(std::string)> CompilationEngine::onWriteOutput;
std::function<void(std::string)> CompilationEngine::onWriteToken;

/// <summary>
/// Opens the output file and gets ready to write into it.
/// <para/> The next routine called must be compileClass.
/// </summary>
/// <param name="filename">Name of the output file</param>
CompilationEngine::CompilationEngine(std::string filename, JackTokenizer* jackTokenizer)
{
    mOutputFileName = filename;
    mOutputFile = new std::ofstream(mOutputFileName);
    if (!mOutputFile->is_open())
    {
        throw std::runtime_error("Cannot find or open " + filename + " file.");
    }
    mJackTokenizer = jackTokenizer;

    if (mWriteTokens)
    {
        fs::path outputPath = filename;
        std::string outputName = outputPath.filename().stem().string();
        outputPath.replace_filename(outputName + "T.xml");

        mTokensFileName = outputPath.string();
        mTokensFile = new std::ofstream(mTokensFileName);
        *mTokensFile << "<tokens>\n";
    }

    onWriteOutput = [this](std::string text)
    {
        *mOutputFile << text;
    };
    onWriteToken = [this](std::string text)
    {
        if (mWriteTokens)
        {
            *mTokensFile << text;
        }
    };
}

/// <summary>
/// Closes the output file.
/// </summary>
CompilationEngine::~CompilationEngine()
{
    if (mOutputFile)
    {
        mOutputFile->close();
        delete mOutputFile;
        mOutputFile = NULL;

        std::cout << "Created " << mOutputFileName << " file." << std::endl;
    }
    if (mTokensFile)
    {
        *mTokensFile << "</tokens>\n";
        mTokensFile->close();
        delete mTokensFile;
        mTokensFile = NULL;

        std::cout << "Created " << mTokensFileName << " file." << std::endl;
    }
    mJackTokenizer = NULL;
}

/// <summary>
/// Compiles a complete class.
/// </summary>
void CompilationEngine::compileClass()
{
    auto classRule = new ClassRule();
    auto result = classRule->initialize(mJackTokenizer);
    if (!result)
    {
        throw std::runtime_error("Failed to initialize class.");
    }
    classRule->compile();
}

void CompilationEngine::writeOutput(std::string text)
{
    onWriteOutput(text);
}

void CompilationEngine::writeToken(std::string text)
{
    onWriteToken(text);
}