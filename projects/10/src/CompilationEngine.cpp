#include <filesystem>
#include <functional>
#include <iostream>
#include "CompilationEngine.h"
#include "JackAnalyzerError.h"
#include "Rules/ProgramStructureRules.h"

using namespace std;
using namespace std::filesystem;

std::function<void(std::string&)> CompilationEngine::onWriteOutput;
std::function<void(std::string&)> CompilationEngine::onWriteToken;

/// <summary>
/// Opens the output file and gets ready to write into it.
/// <para/> The next routine called must be compileClass.
/// </summary>
/// <param name="filename">Name of the output file</param>
CompilationEngine::CompilationEngine(const string& filename, shared_ptr<JackTokenizer> jackTokenizer)
    : mJackTokenizer(jackTokenizer), mOutputFileName(filename)
{
    onWriteOutput = [this](string const& text)
    {
        *mOutputFile << text;
    };
    onWriteToken = [this](string const& text)
    {
        if (mWriteTokens)
        {
            *mTokensFile << text;
        }
    };
}

/// <summary>
/// Compiles a complete class.
/// </summary>
void CompilationEngine::compileClass()
{
    beforeCompile();

    auto classRule = std::make_unique<ClassRule>();
    if (!classRule->initialize(mJackTokenizer.get()))
    {
        throw JackAnalyzerError("Failed to initialize class.");
    }
    classRule->compile();

    afterCompile();
}

void CompilationEngine::beforeCompile()
{
    mOutputFile = make_unique<ofstream>(mOutputFileName);
    if (!mOutputFile->is_open())
    {
        throw JackAnalyzerError("Cannot find or open " + mOutputFileName + " file.");
    }

    if (mWriteTokens)
    {
        path outputPath = mOutputFileName;
        string outputName = outputPath.filename().stem().string();
        outputPath.replace_filename(outputName + "T.xml");

        mTokensFileName = outputPath.string();
        mTokensFile = make_unique<ofstream>(mTokensFileName);
        *mTokensFile << "<tokens>\n";
    }

}

void CompilationEngine::afterCompile()
{
    if (mOutputFile)
    {
        mOutputFile->close();
        mOutputFile = nullptr;
        std::cout << "Created " << mOutputFileName << " file." << std::endl;
    }
    if (mTokensFile)
    {
        *mTokensFile << "</tokens>\n";
        mTokensFile->close();
        mTokensFile = nullptr;
        std::cout << "Created " << mTokensFileName << " file." << std::endl;
    }
}

void CompilationEngine::writeOutput(std::string& text)
{
    onWriteOutput(text);
}

void CompilationEngine::writeToken(std::string& text)
{
    onWriteToken(text);
}