#include <filesystem>
#include <iostream>
#include "CompilationEngine.h"

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
    *mOutputFile << "<class>\n";

    advanceTokenizer();
    if (!(mJackTokenizer->tokenType() == ETokenType::KEYWORD && mJackTokenizer->keyword() == "class"))
    {
        throw std::runtime_error("compileClass - Class should start with 'class' keyword.");
    }
    writeTokenToStreams();

    advanceTokenizer();
    if (!(mJackTokenizer->tokenType() == ETokenType::IDENTIFIER))
    {
        throw std::runtime_error("compileClass - Missing class name.");
    }
    writeTokenToStreams();

    advanceTokenizer();
    if (!(mJackTokenizer->tokenType() == ETokenType::SYMBOL && mJackTokenizer->symbol() == '{'))
    {
        throw std::runtime_error("compileClass - Missing '{' left curly bracket.");
    }
    writeTokenToStreams();

    advanceTokenizer();
    while (mJackTokenizer->tokenType() == ETokenType::KEYWORD && (mJackTokenizer->keyword() == "static" || mJackTokenizer->keyword() == "field"))
    {
        compileClassVarDec();
        advanceTokenizer();
    }

    while (mJackTokenizer->tokenType() == ETokenType::KEYWORD 
        && (mJackTokenizer->keyword() == "constructor" || mJackTokenizer->keyword() == "function" || mJackTokenizer->keyword() == "method"))
    {
        compileSubroutine();
        advanceTokenizer();
    }

    if (!(mJackTokenizer->tokenType() == ETokenType::SYMBOL && mJackTokenizer->symbol() == '}'))
    {
        throw std::runtime_error("compileClass - Missing '}' right curly bracket.");
    }
    writeTokenToStreams();

    *mOutputFile << "</class>";
}

/// <summary>
/// Compiles a static variable declaration or a field declaration.
/// </summary>
void CompilationEngine::compileClassVarDec()
{
    *mOutputFile << "<classVarDec>";
    if (!(mJackTokenizer->tokenType() == ETokenType::KEYWORD && (mJackTokenizer->keyword() == "static" || mJackTokenizer->keyword() == "field")))
    {
        throw std::runtime_error("compileClassVarDec - Missing keyword 'static' or 'field'.");
    }
    writeTokenToStreams();

    advanceTokenizer();
    if (!(mJackTokenizer->tokenType() == ETokenType::KEYWORD && (mJackTokenizer->keyword() == "int" || mJackTokenizer->keyword() == "char" || mJackTokenizer->keyword() == "boolean"))
        && !(mJackTokenizer->tokenType() == ETokenType::IDENTIFIER))
    {
        throw std::runtime_error("compileClassVarDec - Missing valid type for variable declaration.");
    }
    writeTokenToStreams();

    do
    {
        advanceTokenizer();
        if (!(mJackTokenizer->tokenType() == ETokenType::IDENTIFIER))
        {
            throw std::runtime_error("compileClassVarDec - Missing variable name.");
        }
        writeTokenToStreams();

        advanceTokenizer();
        if (!(mJackTokenizer->tokenType() == ETokenType::SYMBOL && mJackTokenizer->symbol() == ','))
        {
            break;
        }
        writeTokenToStreams();
    } while (true);

    if (!(mJackTokenizer->tokenType() == ETokenType::SYMBOL && mJackTokenizer->symbol() == ';'))
    {
        throw std::runtime_error("compileClassVarDec - Missing semi-colon ';' at the end of variable declaration.");
    }
    writeTokenToStreams();

    *mOutputFile << "</classVarDec>";
}

/// <summary>
/// Compiles a complete method, function or constructor.
/// </summary>
void CompilationEngine::compileSubroutine()
{
    *mOutputFile << "<subroutineDec>";

    if (!(mJackTokenizer->tokenType() == ETokenType::KEYWORD
        && (mJackTokenizer->keyword() == "constructor" || mJackTokenizer->keyword() == "function" || mJackTokenizer->keyword() == "method")))
    {
        throw std::runtime_error("compileSubroutine - Missing keyword 'constructor' or 'function' or 'method'.");
    }
    writeTokenToStreams();

    advanceTokenizer();
    if (!(mJackTokenizer->tokenType() == ETokenType::KEYWORD && mJackTokenizer->keyword() == "void")
        && !(mJackTokenizer->tokenType() == ETokenType::IDENTIFIER))
    {
        throw std::runtime_error("compileSubroutine - Missing return type.");
    }
    writeTokenToStreams();

    advanceTokenizer();
    if (!(mJackTokenizer->tokenType() == ETokenType::IDENTIFIER))
    {
        throw std::runtime_error("compileSubroutine - Missing subroutine name.");
    }
    writeTokenToStreams();

    advanceTokenizer();
    if (!(mJackTokenizer->tokenType() == ETokenType::SYMBOL && mJackTokenizer->symbol() == '('))
    {
        throw std::runtime_error("compileSubroutine - Missing left bracket '('.");
    }
    writeTokenToStreams();

    compileParameterList();

    advanceTokenizer();
    if (!(mJackTokenizer->tokenType() == ETokenType::SYMBOL && mJackTokenizer->symbol() == ')'))
    {
        throw std::runtime_error("compileSubroutine - Missing right bracket ')'.");
    }
    writeTokenToStreams();

    compileSubroutineBody();

    *mOutputFile << "</subroutineDec>";
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

void CompilationEngine::advanceTokenizer()
{
    if (!mJackTokenizer->hasMoreTokens())
    {
        throw std::runtime_error("Failed to read token.");
    }

    mJackTokenizer->advance();
}

void CompilationEngine::writeTokenToStreams()
{
    writeTokenToStream(mOutputFile, true);
    writeTokenToStream(mTokensFile, mWriteTokens);
}

void CompilationEngine::writeTokenToStream(std::ofstream* stream, bool writeToken)
{
    if (!writeToken)
    {
        return;
    }

    ETokenType tokenType = mJackTokenizer->tokenType();

    switch (tokenType)
    {
    case ETokenType::KEYWORD:
        *stream << "<keyword> " << mJackTokenizer->keyword() << " </keyword>\n";
        break;
    case ETokenType::SYMBOL:
        *stream << "<symbol> " << encodeXmlSymbol(mJackTokenizer->symbol()) << " </symbol>\n";
        break;
    case ETokenType::IDENTIFIER:
        *stream << "<identifier> " << mJackTokenizer->identifier() << " </identifier>\n";
        break;
    case ETokenType::INT_CONST:
        *stream << "<integerConstant> " << mJackTokenizer->intVal() << " </integerConstant>\n";
        break;
    case ETokenType::STRING_CONST:
        *stream << "<stringConstant> " << mJackTokenizer->stringVal() << " </stringConstant>\n";
        break;
    case ETokenType::UNDEFINED:
        throw std::runtime_error("Undefined token.");
    }
}

std::string CompilationEngine::encodeXmlSymbol(char symbol)
{
    switch (symbol)
    {
    case '<':
        return "&lt;";
    case '>':
        return "&gt;";
    case '"':
        return "&quot;";
    case '&':
        return "&amp;";
    default:
        return std::string(1, symbol);
    }
}
