#include "JackTokenizer.h"

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
	return this->mInputStream->peek() != EOF;
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
