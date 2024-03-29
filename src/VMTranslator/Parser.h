#pragma once
#include <fstream>
#include <string>
#include "ECommandType.h"

/// <summary>
/// The module handles the parsing of a single .vm file.
/// The parser provides services for reading a VM command, unpacking the command into its various components, 
/// and providing convenient access to these components.
/// </summary>
class Parser {
public:
    /// <summary>
    /// Opens the input file and gets ready to parse it.
    /// </summary>
    /// <param name="filename">Name of the input file</param>
    explicit Parser(const std::string& filename);

    /// <summary>
    /// Are there more lines in the input?
    /// </summary>
    bool hasMoreLines();
    /// <summary>
    /// Reads the next command from the input and makes it the current command.
    /// <para/> The routine should be called only if hasMoreLines is true.
    /// <para/> Initially there is no current command.
    /// </summary>
    void advance();
    /// <summary>
    /// Returns a constant representing the type of the current command.
    /// <para/> If the current command is an arithmetic-logical command, returns C_ARITHMETIC.
    /// </summary>
    ECommandType commandType() const;
    /// <summary>
    /// Returns the first argument of the current command.
    /// <para/> In the case of C_ARITHMETIC, the command itself (add, sub, etc.) is returned.
    /// <para/> Should not be called if the current command is C_RETURN.
    /// </summary>
    std::string arg1() const;
    /// <summary>
    /// Returns the second argument of the current command.
    /// <para/> Should be called only if the current command is C_PUSH, C_POP, C_FUNCTION, or C_CALL.
    /// </summary>
    int arg2() const;

private:
    std::unique_ptr<std::ifstream> mInputStream;
    
    std::string mCurrentLine = "";
    ECommandType mCommandType = ECommandType::UNDEFINED;
    std::string mArg1 = "";
    int mArg2 = -1;
};
