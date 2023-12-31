#include <iterator>
#include <sstream>
#include <string>
#include <vector>
#include "Parser.h"
#include "VMTranslatorError.h"

using namespace std;

#pragma region Auxiliary functions
const char* const whitespace_chars = " \t\n\r\f\v";

// trim from end of string (right)
inline string& rtrim(string& s, const char* t = whitespace_chars)
{
    s.erase(s.find_last_not_of(t) + 1);
    return s;
}

// trim from beginning of string (left)
inline string& ltrim(string& s, const char* t = whitespace_chars)
{
    s.erase(0, s.find_first_not_of(t));
    return s;
}

// trim from both ends of string (right then left)
inline string& trim(string& s, const char* t = whitespace_chars)
{
    return ltrim(rtrim(s, t), t);
}

template <typename Out>
void split(const string& s, char delim, Out result)
{
    istringstream iss(s);
    string item;
    while (getline(iss, item, delim))
    {
        if (!item.empty())
        {
            *result++ = item;
        }
    }
}

vector<string> split(const string& s, char delim)
{
    vector<string> elems;
    split(s, delim, back_inserter(elems));
    return elems;
}
#pragma endregion

/// <summary>
/// Opens the input file and gets ready to parse it.
/// </summary>
/// <param name="filename">Name of the input file</param>
Parser::Parser(const string& filename)
{
    mInputStream = make_unique<ifstream>(filename);
    if (!mInputStream->is_open())
    {
        throw VMTranslatorError("Cannot open " + filename + " file");
    }
}

/// <summary>
/// Are there more lines in the input?
/// </summary>
bool Parser::hasMoreLines() 
{
    return mInputStream->peek() != EOF;
}

/// <summary>
/// Reads the next command from the input and makes it the current command.
/// <para/> The routine should be called only if hasMoreLines is true.
/// <para/> Initially there is no current command.
/// </summary>
void Parser::advance()
{
    //initialize fields
    mCurrentLine = "";
    mCommandType = ECommandType::UNDEFINED;
    mArg1 = "";
    mArg2 = -1;

    if (!getline(*mInputStream, mCurrentLine))
        return;

    // remove comments
    if (auto commentPos = mCurrentLine.find("//"); commentPos != string::npos)
    {
        mCurrentLine = commentPos == 0 ? "" : mCurrentLine.substr(0, commentPos);
    }
    mCurrentLine = trim(mCurrentLine);

    if (mCurrentLine == "")
    {
        mCommandType = ECommandType::COMMENT;
        return;
    }

    auto commandArgs = split(mCurrentLine, ' ');
    const auto& command = commandArgs[0];

    if (command == "add" || command == "sub" || command == "neg"
        || command == "eq" || command == "gt" || command == "lt"
        || command == "and" || command == "or" || command == "not")
    {
        mArg1 = command;
        mCommandType = ECommandType::C_ARITHMETIC;
    }
    else if (command == "push" || command == "pop")
    {
        if (commandArgs.size() != 3)
        {
            throw VMTranslatorError("Invalid push/pop command: " + mCurrentLine + ".");
        }
        
        mCommandType = command == "push" ? ECommandType::C_PUSH : ECommandType::C_POP;
        mArg1 = commandArgs[1];
        mArg2 = stoi(commandArgs[2]);
    }
    else if (command == "label")
    {
        mCommandType = ECommandType::C_LABEL;
        mArg1 = commandArgs[1];
    }
    else if (command == "goto")
    {
        mCommandType = ECommandType::C_GOTO;
        mArg1 = commandArgs[1];
    }
    else if (command == "if-goto")
    {
        mCommandType = ECommandType::C_IF;
        mArg1 = commandArgs[1];
    }
    else if (command == "function")
    {
        mCommandType = ECommandType::C_FUNCTION;
        mArg1 = commandArgs[1];
        mArg2 = stoi(commandArgs[2]);
    }
    else if (command == "call")
    {
        mCommandType = ECommandType::C_CALL;
        mArg1 = commandArgs[1];
        mArg2 = stoi(commandArgs[2]);
    }
    else if (command == "return")
    {
        mCommandType = ECommandType::C_RETURN;
    }
    else
    {
        throw VMTranslatorError("Unknown " + command + " command.");
    }
}

/// <summary>
/// Returns a constant representing the type of the current command.
/// <para/> If the current command is an arithmetic-logical command, returns C_ARITHMETIC.
/// </summary>
ECommandType Parser::commandType() const
{
    return mCommandType;
}

/// <summary>
/// Returns the first argument of the current command.
/// <para/> In the case of C_ARITHMETIC, the command itself (add, sub, etc.) is returned.
/// <para/> Should not be called if the current command is C_RETURN.
/// </summary>
string Parser::arg1() const
{
    return mArg1;
}

/// <summary>
/// Returns the second argument of the current command.
/// <para/> Should be called only if the current command is C_PUSH, C_POP, C_FUNCTION, or C_CALL.
/// </summary>
int Parser::arg2() const
{
    return mArg2;
}
