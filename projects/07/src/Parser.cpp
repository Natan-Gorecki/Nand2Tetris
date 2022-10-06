#include "Parser.h"

/// <summary>
/// Are there more lines in the input?
/// </summary>
bool Parser::hasMoreLines() 
{
	return false;
}

/// <summary>
/// Reads the next command from the input and makes it the current command.
/// <para/> The routine should be called only if hasMoreLines is true.
/// <para/> Initially there is no current command.
/// </summary>
void Parser::advance()
{

}

/// <summary>
/// Returns a constant representing the type of the current command.
/// <para/> If the current command is an arithmetic-logical command, returns C_ARITHMETIC.
/// </summary>
ECommandType Parser::commandType()
{
	return ECommandType::UNDEFINED;
}

/// <summary>
/// Returns the first argument of the current command.
/// <para/> In the case of C_ARITHMETIC, the command itself (add, sub, etc.) is returned.
/// <para/> Should not be called if the current command is C_RETURN.
/// </summary>
std::string Parser::arg1()
{
	return std::string();
}

/// <summary>
/// Returns the second argument of the current command.
/// <para/> Should be called only if the current command is C_PUSH, C_POP, C_FUNCTION, or C_CALL.
/// </summary>
int Parser::arg2()
{
	return -1;
}

/// <summary>
/// Opens the input file and gets ready to parse it.
/// </summary>
/// <param name="filename">Name of the input file</param>
Parser::Parser(std::string filename)
{

}

/// <summary>
/// Closes the input file and cleans up.
/// </summary>
Parser::~Parser()
{

}