#pragma once
#include <fstream>
#include <string>
#include "ETokenType.h"

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

public:
	/// <summary>
	/// Are there more tokens in the input?
	/// </summary>
	bool hasMoreTokens();
	/// <summary>
	/// Gets the next token from the input and makes it the current token.
	/// <para/> This method should be called only if hasMoreTokens is true.
	/// <para/> Initially there is no current token.
	/// </summary>
	void advance();
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
	std::ifstream* mInputStream = NULL;
	std::string mCurrentToken = "";

	ETokenType mTokenType = ETokenType::UNDEFINED;
	std::string mKeyWord = "";
	char mSymbol = 0;
	std::string mIdentifier = "";
	int mIntegerValue = 0;
	std::string mStringValue = "";
};