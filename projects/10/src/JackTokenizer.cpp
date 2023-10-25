#include <algorithm>
#include <iostream>
#include "JackTokenizer.h"
#include "Rules/LexicalRules.h"
#include "JackAnalyzerError.h"

using namespace std;

/// <summary>
/// Opens the input file and gets ready to tokenize it.
/// </summary>
/// <param name="filename">Name of the input file</param>
JackTokenizer::JackTokenizer(std::string& filename)
{
    mInputStream = make_unique<ifstream>(filename);
    if (!mInputStream->is_open())
    {
        throw JackAnalyzerError("Cannot open " + filename + " file");
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

        return true;
    }
}

/// <summary>
/// Gets the next token from the input and makes it the current token.
/// <para/> This method should be called only if hasMoreTokens is true.
/// <para/> Initially there is no current token.
/// </summary>
bool JackTokenizer::advance()
{
    auto nextPosition = mCurrentPosition + 1;
    if (nextPosition < mTokens.size())
    {
        mCurrentToken = mTokens.at(nextPosition);
        mCurrentPosition = nextPosition;
        return true;
    }

    if (!hasMoreTokens())
    {
        return false;
    }

    mCurrentToken = Token();
    mCurrentPosition = nextPosition;

    advanceNew();
    mTokens.push_back(mCurrentToken);
    
    return true;
}

/// <summary>
/// Makes the previous token the current token.
/// <para/> If there are no previous tokens, the current token remains unchanged.
/// </summary>
bool JackTokenizer::reverse()
{
    auto previousPosition = mCurrentPosition - 1;
    if (previousPosition < 0)
    {
        return false;
    }

    if (mTokens.size() <= previousPosition)
    {
        return false;
    }

    mCurrentToken = mTokens.at(previousPosition);
    mCurrentPosition = previousPosition;
    return true;
}

/// <summary>
/// Gets the current position of the token in the input.
/// </summary>
/// <returns>The current token position in the input.</returns>
int JackTokenizer::getPosition() const
{
    return mCurrentPosition;
}

/// <summary>
/// Restores the current position to the specified token position.
/// </summary>
/// <param name="position">The token position to set as the current position.</param>
/// <returns>True if the operation was successful, false otherwise.</returns>
bool JackTokenizer::setPosition(int position)
{
    if (position < 0 || position >= mTokens.size())
    {
        return false;
    }

    mCurrentToken = mTokens.at(position);
    mCurrentPosition = position;
    return true;
}

/// <summary>
/// Returns the type of the current token as a constant.
/// </summary>
ETokenType JackTokenizer::tokenType() const
{
    return mCurrentToken.tokenType;
}

/// <summary>
/// Returns the keyword which is the current token as a constant.
/// <para/> This method should be called only if tokenType is KEYWORD.
/// </summary>
std::string JackTokenizer::keyword() const
{
    return mCurrentToken.keyword;
}

/// <summary>
/// Returns the character which is the current token.
/// <para/> Should be called only if tokenType is SYMBOL.
/// </summary>
char JackTokenizer::symbol() const
{
    return mCurrentToken.symbol;
}

/// <summary>
/// Returns the string which is the current token.
/// <para/> Should be called only if tokenType is IDENTIFIER.
/// </summary>
std::string JackTokenizer::identifier() const
{
    return mCurrentToken.identifier;
}

/// <summary>
/// Returns the integer value of the current token.
/// <para/> Should be called only if tokenType is INT_CONST.
/// </summary>
int JackTokenizer::intVal() const
{
    return mCurrentToken.integerValue;
}

/// <summary>
/// Returns the string value of the current token, without opening and closing double quotes.
/// <para/> Should be called only if tokenType is STRING_CONST.
/// </summary>
std::string JackTokenizer::stringVal() const
{
    return mCurrentToken.stringValue;
}

void JackTokenizer::advanceNew()
{
    if (SymbolRule::isSymbol(mFirstChar))
    {
        mCurrentToken.token = std::string(1, mFirstChar);
        mCurrentToken.tokenType = ETokenType::SYMBOL;
        mCurrentToken.symbol = mFirstChar;

        mFirstChar = 0;
        return;
    }

    if (mFirstChar == '"')
    {
        parseStringValue();

        mFirstChar = 0;
        return;
    }

    auto word = readToWhitespaceOrSymbol();

    if (isdigit(word[0]))
    {
        parseIntValue(word);

        mFirstChar = 0;
        return;
    }

    if (KeywordRule::isKeyword(word))
    {
        mCurrentToken.token = word;
        mCurrentToken.tokenType = ETokenType::KEYWORD;
        mCurrentToken.keyword = word;

        mFirstChar = 0;
        return;
    }

    mCurrentToken.token = word;
    mCurrentToken.tokenType = ETokenType::IDENTIFIER;
    mCurrentToken.identifier = word;

    mFirstChar = 0;
    return;
}

std::string JackTokenizer::readToWhitespaceOrSymbol()
{
    auto word = std::string(1, mFirstChar);
    char nextChar = 0;

    while (true)
    {
        nextChar = (char)mInputStream->peek();
        if (nextChar == EOF)
        {
            throw JackAnalyzerError("Failed to read character from input file.");
        }

        if (!std::isalnum(nextChar) && nextChar != '_')
        {
            break;
        }

        if (!mInputStream->get(nextChar))
        {
            throw JackAnalyzerError("Failed to read character from input file.");
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
            throw JackAnalyzerError("Failed to read character from input file.");
        }

        if (nextChar == '"')
        {
            mCurrentToken.token = '"' + line + '"';
            mCurrentToken.tokenType = ETokenType::STRING_CONST;
            mCurrentToken.stringValue = line;
            return;
        }

        if (nextChar == '\r' || nextChar == '\n')
        {
            throw JackAnalyzerError("New line detected. Failed to read string value.");
        }

        line += nextChar;
    }
}

void JackTokenizer::parseIntValue(std::string str)
{
    if (!std::all_of(str.begin(), str.end(), ::isdigit))
    {
        throw JackAnalyzerError("Invalid number - " + str + " .");
    }

    mCurrentToken.token = str;
    mCurrentToken.tokenType = ETokenType::INT_CONST;
    mCurrentToken.integerValue = std::stoi(str.c_str());

    if (mCurrentToken.integerValue < 0 || mCurrentToken.integerValue > SHRT_MAX)
    {
        throw JackAnalyzerError("Number out of scope - " + str + " .");
    }
}

bool JackTokenizer::skipLineComment()
{
    if (mFirstChar != '/')
    {
        return true;
    }

    auto nextChar = (char)mInputStream->peek();
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
    if (mFirstChar != '/')
    {
        return true;
    }

    auto nextChar = mInputStream->peek();
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
