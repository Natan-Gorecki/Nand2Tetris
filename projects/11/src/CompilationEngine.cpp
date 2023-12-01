#include <filesystem>
#include <functional>
#include <iostream>
#include "CompilationEngine.h"
#include "JackCompilerError.h"
#include "Rules/ProgramStructureRules.h"

using namespace std;
using namespace std::filesystem;

/// <summary>
/// Opens the output file and gets ready to write into it.
/// <para/> The next routine called must be compileClass.
/// </summary>
/// <param name="filename">Name of the input file</param>
CompilationEngine::CompilationEngine(const string& filename)
    : mInputFileName(filename)
{
    path path = filename;
    string name = path.filename().stem().string();
    mXmlSyntaxFileName = path.replace_filename(name + ".xml").string();
    mXmlTokensFileName = path.replace_filename(name + "_Tokens.xml").string();
    mVMCodeFileName = path.replace_filename(name + ".vm").string();
}

/// <summary>
/// Compiles a single file.
/// </summary>
void CompilationEngine::compileFile()
{
    beforeCompile();

    auto classRule = std::make_unique<ClassRule>();
    auto jackTokenizer = std::make_unique<JackTokenizer>(mInputFileName);
    if (!classRule->initialize(jackTokenizer.get()))
    {
        throw JackCompilerError("Failed to initialize class.");
    }

    if (mWriteXmlSyntax)
    {
        classRule->writeXmlSyntax(mXmlSyntaxFile.get());
    }
    if (mWriteXmlTokens)
    {
        classRule->writeXmlTokens(mXmlTokensFile.get());
    }
    if (mWriteVMCode)
    {
        classRule->compile(mVMWriter.get());
    }

    afterCompile();
}

void CompilationEngine::beforeCompile()
{
    if (mWriteXmlSyntax)
    {
        mXmlSyntaxFile = make_unique<ofstream>(mXmlSyntaxFileName);
        if (!mXmlSyntaxFile->is_open())
        {
            throw JackCompilerError("Cannot create or open " + mXmlSyntaxFileName + " file.");
        }
    }
    
    if (mWriteXmlTokens)
    {
        mXmlTokensFile = make_unique<ofstream>(mXmlTokensFileName);
        if (!mXmlSyntaxFile->is_open())
        {
            throw JackCompilerError("Cannot create or open " + mXmlTokensFileName + " file.");
        }
        *mXmlTokensFile << "<tokens>\n";
    }

    if (mWriteVMCode)
    {
        mVMCodeFile = make_shared<ofstream>(mVMCodeFileName);
        if (!mVMCodeFile->is_open())
        {
            throw JackCompilerError("Cannot create or open " + mVMCodeFileName + " file.");
        }
        mVMWriter = make_unique<VMWriter>(mVMCodeFile);
    }
}

void CompilationEngine::afterCompile()
{
    if (mWriteXmlSyntax)
    {
        mXmlSyntaxFile->close();
        mXmlSyntaxFile = nullptr;
        std::cout << "Created " << mXmlSyntaxFileName << " file." << std::endl;
    }

    if (mWriteXmlTokens)
    {
        *mXmlTokensFile << "</tokens>\n";
        mXmlTokensFile->close();
        mXmlTokensFile = nullptr;
        std::cout << "Created " << mXmlTokensFileName << " file." << std::endl;
    }

    if (mWriteVMCode)
    {
        mVMCodeFile->close();
        mVMCodeFile = nullptr;
        mVMWriter = nullptr;
        std::cout << "Created " << mVMCodeFileName << " file." << std::endl;
    }
}