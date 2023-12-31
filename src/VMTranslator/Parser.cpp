#include <iterator>
#include <sstream>
#include <string>
#include <vector>

#include "Parser.h"

#pragma region Auxiliary functions
const char* whitespace_chars = " \t\n\r\f\v";

// trim from end of string (right)
inline std::string& rtrim(std::string& s, const char* t = whitespace_chars)
{
    s.erase(s.find_last_not_of(t) + 1);
    return s;
}

// trim from beginning of string (left)
inline std::string& ltrim(std::string& s, const char* t = whitespace_chars)
{
    s.erase(0, s.find_first_not_of(t));
    return s;
}

// trim from both ends of string (right then left)
inline std::string& trim(std::string& s, const char* t = whitespace_chars)
{
    return ltrim(rtrim(s, t), t);
}

template <typename Out>
void split(const std::string& s, char delim, Out result)
{
    std::istringstream iss(s);
    std::string item;
    while (std::getline(iss, item, delim))
    {
        if (!item.empty())
        {
            *result++ = item;
        }
    }
}

std::vector<std::string> split(const std::string& s, char delim)
{
    std::vector<std::string> elems;
    split(s, delim, std::back_inserter(elems));
    return elems;
}
#pragma endregion

/// <summary>
/// Opens the input file and gets ready to parse it.
/// </summary>
/// <param name="filename">Name of the input file</param>
Parser::Parser(std::string filename)
{
    input_stream = new std::ifstream(filename);
    if (!input_stream->is_open())
    {
        throw std::runtime_error("Cannot open " + filename + " file");
    }
}

/// <summary>
/// Closes the input file and cleans up.
/// </summary>
Parser::~Parser()
{
    if (input_stream)
    {
        input_stream->close();
        delete input_stream;
        input_stream = NULL;
    }
}

/// <summary>
/// Are there more lines in the input?
/// </summary>
bool Parser::hasMoreLines() 
{
    return this->input_stream->peek() != EOF;
}

/// <summary>
/// Reads the next command from the input and makes it the current command.
/// <para/> The routine should be called only if hasMoreLines is true.
/// <para/> Initially there is no current command.
/// </summary>
void Parser::advance()
{
    //initialize fields
    current_line = "";
    f_command_type = ECommandType::UNDEFINED;
    f_arg1 = "";
    f_arg2 = -1;

    if (!std::getline(*input_stream, current_line))
        return;

    // remove comments
    size_t comment_pos = current_line.find("//");
    if (comment_pos != std::string::npos)
    {
        current_line = comment_pos == 0 ? "" : current_line.substr(0, comment_pos);
    }
    current_line = trim(current_line);

    if (current_line == "")
    {
        f_command_type = ECommandType::COMMENT;
        return;
    }

    std::vector<std::string> commandArgs = split(current_line, ' ');
    std::string command = commandArgs[0];

    if (command == "add" || command == "sub" || command == "neg"
        || command == "eq" || command == "gt" || command == "lt"
        || command == "and" || command == "or" || command == "not")
    {
        f_arg1 = command;
        f_command_type = ECommandType::C_ARITHMETIC;
    }
    else if (command == "push" || command == "pop")
    {
        if (commandArgs.size() != 3)
        {
            throw std::runtime_error("Invalid push/pop command: " + current_line + ".");
        }
        
        f_command_type = command == "push" ? ECommandType::C_PUSH : ECommandType::C_POP;
        f_arg1 = commandArgs[1];
        f_arg2 = std::stoi(commandArgs[2]);
    }
    else if (command == "label")
    {
        f_command_type = ECommandType::C_LABEL;
        f_arg1 = commandArgs[1];
    }
    else if (command == "goto")
    {
        f_command_type = ECommandType::C_GOTO;
        f_arg1 = commandArgs[1];
    }
    else if (command == "if-goto")
    {
        f_command_type = ECommandType::C_IF;
        f_arg1 = commandArgs[1];
    }
    else if (command == "function")
    {
        f_command_type = ECommandType::C_FUNCTION;
        f_arg1 = commandArgs[1];
        f_arg2 = std::stoi(commandArgs[2]);
    }
    else if (command == "call")
    {
        f_command_type = ECommandType::C_CALL;
        f_arg1 = commandArgs[1];
        f_arg2 = std::stoi(commandArgs[2]);
    }
    else if (command == "return")
    {
        f_command_type = ECommandType::C_RETURN;
    }
    else
    {
        throw std::runtime_error("Unknown " + command + " command.");
    }
}

/// <summary>
/// Returns a constant representing the type of the current command.
/// <para/> If the current command is an arithmetic-logical command, returns C_ARITHMETIC.
/// </summary>
ECommandType Parser::commandType()
{
    return f_command_type;
}

/// <summary>
/// Returns the first argument of the current command.
/// <para/> In the case of C_ARITHMETIC, the command itself (add, sub, etc.) is returned.
/// <para/> Should not be called if the current command is C_RETURN.
/// </summary>
std::string Parser::arg1()
{
    return f_arg1;
}

/// <summary>
/// Returns the second argument of the current command.
/// <para/> Should be called only if the current command is C_PUSH, C_POP, C_FUNCTION, or C_CALL.
/// </summary>
int Parser::arg2()
{
    return f_arg2;
}
