#include "JackTokenizer.h"

/// <summary>
/// Opens the input file and gets ready to tokenize it.
/// </summary>
/// <param name="filename">Name of the input file</param>
JackTokenizer::JackTokenizer(std::string filename)
{

}

/// <summary>
/// Closes input file and cleans up.
/// </summary>
JackTokenizer::~JackTokenizer()
{

}

/// <summary>
/// Are there more tokens in the input?
/// </summary>
bool JackTokenizer::hasMoreTokens()
{

}

/// <summary>
/// Gets the next token from the input and makes it the current token.
/// <para/> This method should be called only if hasMoreTokens is true.
/// <para/> Initially there is no current token.
/// </summary>
void JackTokenizer::advance()
{

}

/// <summary>
/// Returns the type of the current token as a constant.
/// </summary>
ETokenType JackTokenizer::tokenType()
{

}

/// <summary>
/// Returns the keyword which is the current token as a constant.
/// <para/> This method should be called only if tokenType is KEYWORD.
/// </summary>
EKeyWord JackTokenizer::keyWord()
{

}

/// <summary>
/// Returns the character which is the current token.
/// <para/> Should be called only if tokenType is SYMBOL.
/// </summary>
char JackTokenizer::symbol()
{

}

/// <summary>
/// Returns the string which is the current token.
/// <para/> Should be called only if tokenType is IDENTIFIER.
/// </summary>
std::string JackTokenizer::identifier()
{

}

/// <summary>
/// Returns the integer value of the current token.
/// <para/> Should be called only if tokenType is INT_CONST.
/// </summary>
int JackTokenizer::intVal()
{

}

/// <summary>
/// Returns the string value of the current token, without opening and closing double quotes.
/// <para/> Should be called only if tokenType is STRING_CONST.
/// </summary>
std::string JackTokenizer::stringVal()
{

}
