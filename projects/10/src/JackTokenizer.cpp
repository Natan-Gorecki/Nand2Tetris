#include <algorithm>
#include "JackTokenizer.h"
#include "KeywordTable.h"
#include "SymbolTable.h"

/// <summary>
/// Opens the input file and gets ready to tokenize it.
/// </summary>
/// <param name="filename">Name of the input file</param>
JackTokenizer::JackTokenizer(std::string filename)
{
    mInputStream = new std::ifstream(filename);
    if (!mInputStream->is_open())
    {
        throw std::runtime_error("Cannot open " + filename + " file");
    }
}

/// <summary>
/// Closes input file and cleans up.
/// </summary>
JackTokenizer::~JackTokenizer()
{
    if (mInputStream)
    {
        mInputStream->close();
        delete mInputStream;
        mInputStream = NULL;
    }
}

/// <summary>
/// Are there more tokens in the input?
/// </summary>
bool JackTokenizer::hasMoreTokens()
{
    while (true)
    {
        if (!mInputStream->get(mFirstChar))
        {
            return false;
        }

        if (!skipLineComment())
        {
            return false;
        }

        if (!skipMultilineComment())
        {
            return false;
        }

        if (isspace(mFirstChar))
        {
            continue;
        }

        if (mFirstChar == '"')
        {
            mDoubleQuotesStarted = !mDoubleQuotesStarted;
        }
        return true;
    }
}

/// <summary>
/// Gets the next token from the input and makes it the current token.
/// <para/> This method should be called only if hasMoreTokens is true.
/// <para/> Initially there is no current token.
/// </summary>
void JackTokenizer::advance()
{
    resetFieldValues();
    
    if (mFirstChar == 0)
    {
        throw std::runtime_error("mFirstChar has default value. Call hasMoreTokens before advance.");
    }
    
    if (SymbolTable::isSymbol(mFirstChar))
    {
        mToken = std::string(1, mFirstChar);
        mTokenType = ETokenType::SYMBOL;
        mSymbol = mFirstChar;
        
        mFirstChar = 0;
        return;
    }

    if (mFirstChar == '"')
    {
        parseStringValue();
        
        mFirstChar = 0;
        return;
    }
    
    std::string word = readToWhitespaceOrSymbol();
    
    if (isdigit(word[0]))
    {
        parseIntValue(word);
        
        mFirstChar = 0;
        return;
    }
    
    if (KeywordTable::isKeyword(word))
    {
        mToken = word;
        mTokenType = ETokenType::KEYWORD;
        mKeyWord = word;

        mFirstChar = 0;
        return;
    }

    mToken = word;
    mTokenType = ETokenType::IDENTIFIER;
    mIdentifier = word;

    mFirstChar = 0;
    return;
}

/// <summary>
/// Returns the type of the current token as a constant.
/// </summary>
ETokenType JackTokenizer::tokenType()
{
    return mTokenType;
}

/// <summary>
/// Returns the keyword which is the current token as a constant.
/// <para/> This method should be called only if tokenType is KEYWORD.
/// </summary>
std::string JackTokenizer::keyword()
{
    return mKeyWord;
}

/// <summary>
/// Returns the character which is the current token.
/// <para/> Should be called only if tokenType is SYMBOL.
/// </summary>
char JackTokenizer::symbol()
{
    return mSymbol;
}

/// <summary>
/// Returns the string which is the current token.
/// <para/> Should be called only if tokenType is IDENTIFIER.
/// </summary>
std::string JackTokenizer::identifier()
{
    return mIdentifier;
}

/// <summary>
/// Returns the integer value of the current token.
/// <para/> Should be called only if tokenType is INT_CONST.
/// </summary>
int JackTokenizer::intVal()
{
    return mIntegerValue;
}

/// <summary>
/// Returns the string value of the current token, without opening and closing double quotes.
/// <para/> Should be called only if tokenType is STRING_CONST.
/// </summary>
std::string JackTokenizer::stringVal()
{
    return mStringValue;
}

void JackTokenizer::resetFieldValues()
{
    mToken = "";
    mTokenType = ETokenType::UNDEFINED;

    mKeyWord = "";
    mSymbol = 0;
    mIdentifier = "";
    mIntegerValue = 0;
    mStringValue = "";
}

std::string JackTokenizer::readToWhitespaceOrSymbol()
{
    std::string word = std::string(1, mFirstChar);
    char nextChar = 0;

    while (true)
    {
        nextChar = mInputStream->peek();
        if (nextChar == EOF)
        {
            throw std::runtime_error("Failed to read character from input file.");
        }

        if (!std::isalnum(nextChar) && nextChar != '_')
        {
            break;
        }

        if (!mInputStream->get(nextChar))
        {
            throw std::runtime_error("Failed to read character from input file.");
        }

        word += nextChar;
    }

    return word;
}

void JackTokenizer::parseStringValue()
{
    std::string line = "";
    char nextChar = 0;

    while (true)
    {
        if (!mInputStream->get(nextChar))
        {
            throw std::runtime_error("Failed to read character from input file.");
        }

        if (nextChar == '"')
        {
            mToken = '"' + line + '"';
            mTokenType = ETokenType::STRING_CONST;
            mStringValue = line;
            return;
        }

        if (nextChar == '\r' || nextChar == '\n')
        {
            throw std::runtime_error("New line detected. Failed to read string value.");
        }

        line += nextChar;
    }
}

void JackTokenizer::parseIntValue(std::string str)
{
    bool isDigits = std::all_of(str.begin(), str.end(), ::isdigit);
    if (!isDigits)
    {
        throw std::runtime_error("Invalid number - " + str + " .");
    }

    mToken = str;
    mTokenType = ETokenType::INT_CONST;
    mIntegerValue = std::stoi(str.c_str());

    if (mIntegerValue < 0 || mIntegerValue > SHRT_MAX)
    {
        throw std::runtime_error("Number out of scope - " + str + " .");
    }
}

bool JackTokenizer::skipLineComment()
{
    if (mFirstChar != '/' || mDoubleQuotesStarted)
    {
        return true;
    }

    char nextChar = mInputStream->peek();
    if (nextChar == EOF)
    {
        return false;
    }
    if (nextChar != '/')
    {
        return true;
    }

    while (mFirstChar != '\n')
    {
        if (!mInputStream->get(mFirstChar))
        {
            return false;
        }
    }

    if (!mInputStream->get(mFirstChar))
    {
        return false;
    }

    return skipLineComment();
}

bool JackTokenizer::skipMultilineComment()
{
    if (mFirstChar != '/' || mDoubleQuotesStarted)
    {
        return true;
    }

    char nextChar = mInputStream->peek();
    if (nextChar == EOF)
    {
        return false;
    }
    if (nextChar != '*')
    {
        return true;
    }

    if (!mInputStream->get(mFirstChar))
    {
        return false;
    }

    char previousChar;
    while (true)
    {
        previousChar = mFirstChar;
        if (!mInputStream->get(mFirstChar))
        {
            return false;
        }

        if (previousChar == '*' && mFirstChar == '/')
        {
            break;
        }
    }

    if (!mInputStream->get(mFirstChar))
    {
        return false;
    }

    return skipMultilineComment();
}
