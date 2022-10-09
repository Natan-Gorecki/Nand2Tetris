#include "CodeWriter.h"
#include <iostream>
#include <regex>

/// <summary>
/// Opens the output file and gets ready to write into it
/// </summary>
/// <param name="filename">Name of the output file</param>
/// <param name="generateComment">Information if comment should be generated</param>
CodeWriter::CodeWriter(std::string filename, bool generateComment)
{
    this->output_file = new std::ofstream(filename);
    if (!this->output_file->is_open())
    {
        throw std::runtime_error("Cannot find or open " + filename + " file.");
    }

    this->generate_comment = generateComment;

    initialCode();
}

/// <summary>
/// Closes the output file
///</summary>
CodeWriter::~CodeWriter()
{
    if (output_file)
    {
        output_file->close();
        delete output_file;
        output_file = NULL;
    }
}

/// <summary>
/// Writes to the output file the assembly code that implements the given arithmetic-logical command.
/// </summary>
/// <param name="command">Arithmetic-logical command</param>
void CodeWriter::writeArithmetic(std::string command)
{
    *output_file << generate_comment ? "// " + command + "\n" : "";

    if (command == "add")
    {
        const char* addCommand = 
R"(@SP
AM=M-1
D=M
A=A-1
M=M+D)";
        *output_file << addCommand << "\n";
    }
    else if (command == "sub") 
    {
        const char* subCommand =
R"(@SP
AM=M-1
D=M
A=A-1
M=M-D)";
        *output_file << subCommand << "\n";
    }
    else if (command == "neg")
    {
        const char* negCommand =
R"(@SP
A=M-1
M=-M)";
        *output_file << negCommand << "\n";
    }
    else if (command == "eq")
    {
        const char* eqCommand =
R"(@AFTER_CONDITION.1
D=A
@R13
M=D

@SP
AM=M-1
D=M

@SP
AM=M-1
D=D-M

@TRUE_CONDITION
D;JEQ
@FALSE_CONDITION
0;JMP

(AFTER_CONDITION.1))";
        std::string eqString(eqCommand);
        eqString = std::regex_replace(eqString, std::regex("AFTER_CONDITION.1"), "AFTER_CONDITION." + getUniqNumber());
        *output_file << eqString << "\n";
    }
    else if (command == "gt")
    {
        const char* gtCommand =
R"(@AFTER_CONDITION.1
D=A
@R13
M=D

@SP
AM=M-1
D=M

@SP
AM=M-1
D=M-D

@TRUE_CONDITION
D;JGT
@FALSE_CONDITION
0;JMP

(AFTER_CONDITION.1))";
        std::string gtString(gtCommand);
        gtString = std::regex_replace(gtString, std::regex("AFTER_CONDITION.1"), "AFTER_CONDITION." + getUniqNumber());
        *output_file << gtString << "\n";
    }
    else if (command == "lt")
    {
        const char* ltCommand =
R"(@AFTER_CONDITION.1
D=A
@R13
M=D

@SP
AM=M-1
D=M

@SP
AM=M-1
D=M-D

@TRUE_CONDITION
D;JLT
@FALSE_CONDITION
0;JMP

(AFTER_CONDITION.1))";
        std::string ltString(ltCommand);
        ltString = std::regex_replace(ltString, std::regex("AFTER_CONDITION.1"), "AFTER_CONDITION." + getUniqNumber());
        *output_file << ltString << "\n";
    }
    else if (command == "and")
    {
    const char* andCommand =
R"(@SP
AM=M-1
D=M
A=A-1
M=M&D)";
    *output_file << andCommand << "\n";
    }
    else if (command == "or")
    {
    const char* orCommand =
R"(@SP
AM=M-1
D=M
A=A-1
M=M|D)";
    *output_file << orCommand << "\n";
    }
    else if (command == "not")
    {
    const char* notCommand =
R"(@SP
A=M-1
M=!M)";
    *output_file << notCommand << "\n";
    }
    else
    {
        throw std::runtime_error("Unknown C_ARITHMETIC " + command + " command.");
    }    
}

/// <summary>
/// Writes to the output file the assembly code that implements the given push or pop command
/// </summary>
/// <param name="commandType">C_PUSH or C_POP</param>
/// <param name="segment">Name of the segment</param>
/// <param name="index">Numeric address</param>
void CodeWriter::writePushPop(ECommandType commandType, std::string segment, int index)
{
    if (commandType == ECommandType::C_PUSH)
    {
        this->writePush(segment, index);
    }
    else if (commandType == ECommandType::C_POP)
    {
        this->writePop(segment, index);
    }
    else
    {
        throw std::runtime_error("Method only allowed with C_PUSH or C_POP command.");
    }
}

void CodeWriter::initialCode()
{
    const char* eqCommand =
R"((TRUE_CONDITION)
@-1
D=A
@SP
A=M
M=D
@SP
M=M+1
@R13
0;JMP
(FALSE_CONDITION)
@0
D=A
@SP
A=M
M=D
@SP
M=M+1
@R13
0;JMP)";
    *output_file << eqCommand << "\n";
}

void CodeWriter::writePush(std::string segment, int index)
{
    if (segment == "local")
    {
        const char* pushCommand =
R"(@INDEX
D=A
@LCL
A=A+D
D=M

@SP
A=M
M=D

@SP
M=M+1)";
        std::string pushString(pushCommand);
        pushString = std::regex_replace(pushString, std::regex("INDEX"), std::to_string(index));
        *output_file << pushString << "\n";
    }
    else if (segment == "argument")
    {
        const char* pushCommand =
R"(@INDEX
D=A
@ARG
A=A+D
D=M

@SP
A=M
M=D

@SP
M=M+1)";
        std::string pushString(pushCommand);
        pushString = std::regex_replace(pushString, std::regex("INDEX"), std::to_string(index));
        *output_file << pushString << "\n";
    }
    else if (segment == "this")
    {
        const char* pushCommand =
R"(@INDEX
D=A
@THIS
A=A+D
D=M

@SP
A=M
M=D

@SP
M=M+1)";
        std::string pushString(pushCommand);
        pushString = std::regex_replace(pushString, std::regex("INDEX"), std::to_string(index));
        *output_file << pushString << "\n";
    }
    else if (segment == "that")
    {
        const char* pushCommand =
R"(@INDEX
D=A
@THAT
A=A+D
D=M

@SP
A=M
M=D

@SP
M=M+1)";
        std::string pushString(pushCommand);
        pushString = std::regex_replace(pushString, std::regex("INDEX"), std::to_string(index));
        *output_file << pushString << "\n";
    }
    else if (segment == "pointer")
    {
        if (index == 0)
        {
            const char* pushCommand =
R"(@THIS
D=M

@SP
A=M
M=D

@SP
M=M+1)";
            *output_file << pushCommand << "\n";
        }
        else if (index == 1)
        {
            const char* pushCommand =
R"(@THAT
D=M

@SP
A=M
M=D

@SP
M=M+1)";
            *output_file << pushCommand << "\n";
        }
        else
        {
            throw std::runtime_error("Unsupported index " + std::to_string(index) + " with 'push pointer' command.");
        }
    }
    else if (segment == "temp")
    {
        int tempIndex = 5 + index;
        if (index < 0 || index > 7)
        {
            throw std::runtime_error("Unsupported index " + std::to_string(index) + " with 'push temp' command.");
        }

        const char* pushCommand =
R"(@INDEX
D=M

@SP
A=M
M=D

@SP
M=M+1)";
        std::string pushString(pushCommand);
        pushString = std::regex_replace(pushString, std::regex("INDEX"), "R" + std::to_string(tempIndex));
        *output_file << pushString << "\n";
    }
    else if (segment == "constant")
    {
        if (index < 0 || index > SHRT_MAX)
        {
            throw std::runtime_error("Unsupported index " + std::to_string(index) + " with 'push constant' command.");
        }

        const char* pushCommand =
R"(@INDEX
D=A

@SP
A=M
M=D

@SP
M=M+1)";
        std::string pushString(pushCommand);
        pushString = std::regex_replace(pushString, std::regex("INDEX"), std::to_string(index));
        *output_file << pushString << "\n";
    }
    else if (segment == "static")
    {
        const char* pushCommand =
R"(@Foo.INDEX
D=M

@SP
A=M
M=D

@SP
M=M+1)";
        std::string pushString(pushCommand);
        pushString = std::regex_replace(pushString, std::regex("INDEX"), std::to_string(index));
        *output_file << pushString << "\n";
    }
    else
    {
        throw std::runtime_error("Unknown " + segment + " segment.");
    }
}

void CodeWriter::writePop(std::string segment, int index)
{
    if (segment == "local")
    {
        const char* popCommand =
R"(@INDEX
D=A
@LCL
D=A+D

@R14
M=D

@SP
AM=M-1
D=M

@R14
A=M
M=D)";
        std::string popString(popCommand);
        popString = std::regex_replace(popString, std::regex("INDEX"), std::to_string(index));
        *output_file << popString << "\n";
    }
    else if (segment == "argument")
    {
        const char* popCommand =
R"(@INDEX
D=A
@ARG
D=A+D

@R14
M=D

@SP
AM=M-1
D=M

@R14
A=M
M=D)";
        std::string popString(popCommand);
        popString = std::regex_replace(popString, std::regex("INDEX"), std::to_string(index));
        *output_file << popString << "\n";
    }
    else if (segment == "this")
    {
        const char* popCommand =
R"(@INDEX
D=A
@THIS
D=A+D

@R14
M=D

@SP
AM=M-1
D=M

@R14
A=M
M=D)";
        std::string popString(popCommand);
        popString = std::regex_replace(popString, std::regex("INDEX"), std::to_string(index));
        *output_file << popString << "\n";
    }
    else if (segment == "that")
    {
        const char* popCommand =
R"(@INDEX
D=A
@THAT
D=A+D

@R14
M=D

@SP
AM=M-1
D=M

@R14
A=M
M=D)";
        std::string popString(popCommand);
        popString = std::regex_replace(popString, std::regex("INDEX"), std::to_string(index));
        *output_file << popString << "\n";
    }
    else if (segment == "pointer")
    {
        if (index == 0)
        {
            const char* popCommand =
R"(@SP
AM=M-1
D=M

@THIS
M=D)";
            *output_file << popCommand << "\n";
        }
        else if (index == 1)
        {
            const char* popCommand =
R"(@SP
AM=M-1
D=M

@THAT
M=D)";
            *output_file << popCommand << "\n";
        }
        else
        {
            throw std::runtime_error("Unsupported index " + std::to_string(index) + " with 'pop pointer' command.");
        }
    }
    else if (segment == "temp")
    {
        int tempIndex = 5 + index;
        if (index < 0 || index > 7)
        {
            throw std::runtime_error("Unsupported index " + std::to_string(index) + " with 'pop temp' command.");
        }

        const char* popCommand =
R"(@SP
AM=M-1
D=M

@INDEX
M=D)";
        std::string popString(popCommand);
        popString = std::regex_replace(popString, std::regex("INDEX"), "R" + std::to_string(tempIndex));
        *output_file << popString << "\n";
    }
    else if (segment == "constant")
    {
        throw std::runtime_error("Unsupported command 'pop constant " + std::to_string(index) + "'.");
    }
    else if (segment == "static")
    {
        const char* popCommand =
R"(@SP
AM=M-1
D=M

@Foo.INDEX
D=M)";
        std::string popString(popCommand);
        popString = std::regex_replace(popString, std::regex("INDEX"), std::to_string(index));
        *output_file << popString << "\n";
    }
    else
    {
        throw std::runtime_error("Unknown " + segment + " segment.");
    }
}

int CodeWriter::getUniqNumber()
{
    return uniq_number++;
}