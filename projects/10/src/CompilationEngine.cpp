#include <filesystem>
#include <functional>
#include <iostream>
#include "Assert.h"
#include "CompilationEngine.h"
#include "Rules/ProgramStructureRules.h"

namespace fs = std::filesystem;

/// <summary>
/// Opens the output file and gets ready to write into it.
/// <para/> The next routine called must be compileClass.
/// </summary>
/// <param name="filename">Name of the output file</param>
CompilationEngine::CompilationEngine(std::string filename, JackTokenizer* jackTokenizer)
{
    mOutputFile = new std::ofstream(filename);
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
        *mTokensFile << text;
    };
    onAdvanceToken = [this]()
    {
        if (!mJackTokenizer->hasMoreTokens())
        {
            throw std::runtime_error("Failed to read token.");
        }

        mJackTokenizer->advance();
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
    classRule->compile(mJackTokenizer);
}

/// <summary>
/// Compiles a static variable declaration or a field declaration.
/// </summary>
void CompilationEngine::compileClassVarDec()
{
    /**mOutputFile << "<classVarDec>";

    if (!Assert::That(ETokenType::KEYWORD, "static", "field"))
    {
        throw new std::runtime_error("compileClassVarDec - Missing keyword 'static' or 'field'.");
    }
    writeTokenToStreams();

    advanceToken();
    if (!Assert::That(ETokenType::KEYWORD, "int", "char", "boolean") && !Assert::That(ETokenType::IDENTIFIER))
    {
        throw std::runtime_error("compileType - Missing valid type: 'int', 'char', 'boolean' or class name.");
    }
    writeTokenToStreams();

    do
    {
        advanceToken();
        if (!Assert::That(ETokenType::IDENTIFIER))
        {
            throw std::runtime_error("compileClassVarDec - Missing variable name.");
        }
        writeTokenToStreams();

        advanceToken();
        if (!Assert::That(ETokenType::SYMBOL, ','))
        {
            break;
        }
        writeTokenToStreams();
    } while (true);

    if (!Assert::That(ETokenType::SYMBOL, ';'))
    {
        throw std::runtime_error("compileClassVarDec - Missing semi-colon ';' at the end of variable declaration.");
    }

    *mOutputFile << "</classVarDec>";*/
}

/// <summary>
/// Compiles a complete method, function or constructor.
/// </summary>
void CompilationEngine::compileSubroutine()
{
    /**mOutputFile << "<subroutineDec>";

    if (!Assert::That(ETokenType::KEYWORD, "constructor", "function", "method"))
    {
        throw std::runtime_error("compileSubroutine - Missing keyword 'constructor' or 'function' or 'method'.");
    }    
    writeTokenToStreams();



    advanceToken();
    if (!Assert::That(ETokenType::KEYWORD, "void") && !Assert::That(ETokenType::IDENTIFIER))
    {
        throw std::runtime_error("compileSubroutine - Missing return type.");
    }
    writeTokenToStreams();

    advanceToken();
    if (!Assert::That(ETokenType::IDENTIFIER))
    {
        throw std::runtime_error("compileSubroutine - Missing subroutine name.");
    }
    writeTokenToStreams();

    advanceToken();
    if (!Assert::That(ETokenType::SYMBOL, '('))
    {
        throw std::runtime_error("compileSubroutine - Missing left bracket '('.");
    }
    writeTokenToStreams();

    compileParameterList();

    advanceToken();
    if (!Assert::That(ETokenType::SYMBOL, ')'))
    {
        throw std::runtime_error("compileSubroutine - Missing right bracket ')'.");
    }
    writeTokenToStreams();

    compileSubroutineBody();

    *mOutputFile << "</subroutineDec>";*/
}

/// <summary>
/// Compiles a (possibly empty) parameter list. Does not handle the enclosing parentheses tokens ( and ).
/// </summary>
void CompilationEngine::compileParameterList()
{

}

/// <summary>
/// Compiles a subroutine's body.
/// </summary>
void CompilationEngine::compileSubroutineBody()
{

}

/// <summary>
/// Compiles a var declaration.
/// </summary>
void CompilationEngine::compileVarDec()
{

}

/// <summary>
/// Compiles a sequence of statements. Does not handle the enclosing curly bracket tokens { and }.
/// </summary>
void CompilationEngine::compileStatements()
{

}

/// <summary>
/// Compiles a let statement.
/// </summary>
void CompilationEngine::compileLet()
{

}

/// <summary>
/// Compiles an if statement, possibly with a trailing else clause.
/// </summary>
void CompilationEngine::compileIf()
{

}

/// <summary>
/// Compiles a while statement.
/// </summary>
void CompilationEngine::compileWhile()
{

}

/// <summary>
/// Compiles a do statement.
/// </summary>
void CompilationEngine::compileDo()
{

}

/// <summary>
/// Compiles a return statement.
/// </summary>
void CompilationEngine::compileReturn()
{

}

/// <summary>
/// Compiles an expression.
/// </summary>
void CompilationEngine::compileExpression()
{

}

/// <summary>
/// Compiles a term. If the current token is an identifier, the routine must resolve it
///     into a variable, an array element or a subroutine call.
/// A single lookahead token, which may be [, ( or . suffices to distinguish between the possibilities.
/// Any other token is not part of this term and should not be advanced over.
/// </summary>
void CompilationEngine::compileTerm()
{

}

/// <summary>
/// Compiles a (possibly empty) comma-separated list of expressions.
/// </summary>
/// <returns>Returns the number of expressions in the list</returns>
int CompilationEngine::compileExpressionList()
{
    return 0;
}

void CompilationEngine::writeOutput(std::string text)
{
    onWriteOutput(text);
}

void CompilationEngine::writeToken(std::string text)
{
    onWriteToken(text);
}

void CompilationEngine::advanceToken()
{
    onAdvanceToken();
}