#pragma once
#include <fstream>
#include <string>
#include <vector>
#include "ETokenType.h"

struct Token
{
    std::string token = "";
    ETokenType tokenType = ETokenType::UNDEFINED;

    std::string keyword = "";
    char symbol = 0;
    std::string identifier = "";
    int integerValue = 0;
    std::string stringValue = "";
};

/// <summary>
/// The module ignores all comments and white space in the input stream and enables accessing the input one token at a time.
/// Also, it parses and provides the type of each token, as defined by the Jack grammar.
/// </summary>
class JackTokenizer
{
public:
    /// <summary>
    /// Opens the input file and gets ready to tokenize it.
    /// </summary>
    /// <param name="filename">Name of the input file</param>
    JackTokenizer(std::string filename);
    /// <summary>
    /// Closes input file and cleans up.
    /// </summary>
    ~JackTokenizer();

private:
    /// <summary>
    /// Are there more tokens in the input?
    /// </summary>
    bool hasMoreTokens();
public:
    /// <summary>
    /// Gets the next token from the input and makes it the current token.
    /// <para/> Initially there is no current token.
    /// </summary>
    bool advance();
    /// <summary>
    /// Makes the previous token the current token.
    /// <para/> If there are no previous tokens, the current token remains unchanged.
    /// </summary>
    bool reverse();
    /// <summary>
    /// Gets the current position of the token in the input.
    /// </summary>
    /// <returns>The current token position in the input.</returns>
    int getPosition();
    /// <summary>
    /// Restores the current position to the specified token position.
    /// </summary>
    /// <param name="position">The token position to set as the current position.</param>
    /// <returns>True if the operation was successful, false otherwise.</returns>
    bool setPosition(int position);
    /// <summary>
    /// Returns the type of the current token as a constant.
    /// </summary>
    ETokenType tokenType();
    /// <summary>
    /// Returns the keyword which is the current token as a constant.
    /// <para/> This method should be called only if tokenType is KEYWORD.
    /// </summary>
    std::string keyword();
    /// <summary>
    /// Returns the character which is the current token.
    /// <para/> Should be called only if tokenType is SYMBOL.
    /// </summary>
    char symbol();
    /// <summary>
    /// Returns the string which is the current token.
    /// <para/> Should be called only if tokenType is IDENTIFIER.
    /// </summary>
    std::string identifier();
    /// <summary>
    /// Returns the integer value of the current token.
    /// <para/> Should be called only if tokenType is INT_CONST.
    /// </summary>
    int intVal();
    /// <summary>
    /// Returns the string value of the current token, without opening and closing double quotes.
    /// <para/> Should be called only if tokenType is STRING_CONST.
    /// </summary>
    std::string stringVal();

private:
    void advanceNew();
    std::string readToWhitespaceOrSymbol();
    void parseStringValue();
    void parseIntValue(std::string str);

private:
    bool skipLineComment();
    bool skipMultilineComment();

private:
    std::ifstream* mInputStream = NULL;
    char mFirstChar = 0;
    
    std::vector<Token> mTokens;
    int mCurrentPosition = -1;
    Token mCurrentToken;
};
