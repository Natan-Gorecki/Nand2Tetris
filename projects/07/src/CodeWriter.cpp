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
        this->finalCode();
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
    if (generate_comment)
    {
        *output_file << "\n// " << command << "\n";
    }

    if (command == "add")
    {
        const char* addCommand = 
R"(@SP
AM=M-1
D=M
A=A-1
M=M+D
)";
        *output_file << addCommand;
    }
    else if (command == "sub") 
    {
        const char* subCommand =
R"(@SP
AM=M-1
D=M
A=A-1
M=M-D
)";
        *output_file << subCommand;
    }
    else if (command == "neg")
    {
        const char* negCommand =
R"(@SP
A=M-1
M=-M
)";
        *output_file << negCommand;
    }
    else if (command == "eq")
    {
        writeComparisonCommand("D;JEQ");
    }
    else if (command == "gt")
    {
        writeComparisonCommand("D;JGT");
    }
    else if (command == "lt")
    {
        writeComparisonCommand("D;JLT");
    }
    else if (command == "and")
    {
        const char* andCommand =
R"(@SP
AM=M-1
D=M
A=A-1
M=M&D
)";
        *output_file << andCommand;
    }
    else if (command == "or")
    {
        const char* orCommand =
R"(@SP
AM=M-1
D=M
A=A-1
M=M|D
)";
        *output_file << orCommand;
    }
    else if (command == "not")
    {
        const char* notCommand =
R"(@SP
A=M-1
M=!M
)";
        *output_file << notCommand;
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
    std::string pushPop = commandType == ECommandType::C_PUSH ? "push" : "pop";
    if (generate_comment)
    {
        *output_file << "\n// " << pushPop << " " << segment << " " << index << "\n";
    }
    
    if (commandType != ECommandType::C_PUSH && commandType != ECommandType::C_POP)
    {
        throw std::runtime_error("Method only allowed with C_PUSH or C_POP command.");
    }

    if (segment == "local")
    {
        writeLocalArgThisThat(commandType, "LCL", index);
    }
    else if (segment == "argument")
    {
        writeLocalArgThisThat(commandType, "ARG", index);
    }
    else if (segment == "this")
    {
        writeLocalArgThisThat(commandType, "THIS", index);
    }
    else if (segment == "that")
    {
        writeLocalArgThisThat(commandType, "THAT", index);
    }
    else if (segment == "pointer")
    {
        writePointer(commandType, index);
    }
    else if (segment == "temp")
    {
        writeTemp(commandType, index);
    }
    else if (segment == "constant")
    {
        writeConstant(commandType, index);
    }
    else if (segment == "static")
    {
        writeStatic(commandType, index);
    }
    else
    {
        throw std::runtime_error("Unknown " + segment + " segment.");
    }
}

void CodeWriter::initialCode()
{
    const char* initCode =
R"((START)
@256
D=A
@SP
M=D
)";
    *output_file << initCode;
}

void CodeWriter::finalCode()
{
    const char* finalCommand =
R"(
(END)
@END
0;JMP
)";
    *output_file << finalCommand;

    if (is_comparison_used)
    {
        const char* comparisonCommand =
R"(
(TRUE_CONDITION)
@1
A=-A
D=A
DREGISTER_2_STACK
@R13
A=M
0;JMP

(FALSE_CONDITION)
@0
D=A
DREGISTER_2_STACK
@R13
A=M
0;JMP
)";
        std::string comparisonString = std::string(comparisonCommand);
        comparisonString = std::regex_replace(comparisonString, std::regex("DREGISTER_2_STACK\n"), DRegister2Stack());
        *output_file << comparisonString;
    }
}

void CodeWriter::writeComparisonCommand(std::string comparisonCheck)
{
    is_comparison_used = true;

    const char* comparisonCommand =
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
COMPARISON_CHECK
@FALSE_CONDITION
0;JMP
(AFTER_CONDITION.1)
)";
    std::string comparisonString(comparisonCommand);
    comparisonString = std::regex_replace(comparisonString, std::regex("AFTER_CONDITION\\.1"), std::string("AFTER_CONDITION.") + std::to_string(getUniqNumber()));
    comparisonString = std::regex_replace(comparisonString, std::regex("COMPARISON_CHECK"), comparisonCheck);
    *output_file << comparisonString;
}

void CodeWriter::writeLocalArgThisThat(ECommandType commandType, std::string segmentName, int index)
{
    if (commandType == ECommandType::C_PUSH)
    {
        const char* pushCommand =
R"(@INDEX
D=A
@SEGMENT_NAME
A=M+D
D=M
)";
        std::string pushString = std::string(pushCommand);
        pushString = std::regex_replace(pushString, std::regex("INDEX"), std::to_string(index));
        pushString = std::regex_replace(pushString, std::regex("SEGMENT_NAME"), segmentName);
        *output_file 
            << pushString 
            << DRegister2Stack();
    }
    else if (commandType == ECommandType::C_POP)
    {
        const char* address2temp =
R"(@INDEX
D=A
@SEGMENT_NAME
D=M+D
@R14
M=D
)";
        const char* value2Address =
R"(@R14
A=M
M=D
)";
        std::string popString(address2temp);
        popString = std::regex_replace(popString, std::regex("INDEX"), std::to_string(index));
        popString = std::regex_replace(popString, std::regex("SEGMENT_NAME"), segmentName);
        *output_file 
            << popString 
            << stack2DRegister() 
            << value2Address;
    }
}

void CodeWriter::writePointer(ECommandType commandType, int index)
{
    if (index != 0 && index != 1)
    {
        throw std::runtime_error("Unsupported index " + std::to_string(index) + " with pointer segment.");
    }

    const char* segmentName = index == 0 ? "THIS" : "THAT";

    if (commandType == ECommandType::C_PUSH)
    {
        *output_file
            << "@" << segmentName << "\n"
            << "D=M\n"
            << DRegister2Stack();
    }
    else if (commandType == ECommandType::C_POP)
    {
        *output_file
            << stack2DRegister()
            << "@" << segmentName << "\n"
            << "M=D\n";
    }
}

void CodeWriter::writeTemp(ECommandType commandType, int index)
{
    int tempIndex = 5 + index;
    if (index < 0 || index > 7)
    {
        throw std::runtime_error("Unsupported index " + std::to_string(index) + " with temp segment.");
    }

    if (commandType == ECommandType::C_PUSH)
    {
        *output_file
            << "@R" << tempIndex << "\n"
            << "D=M\n"
            << DRegister2Stack();
    }
    else if (commandType == ECommandType::C_POP)
    {
        *output_file
            << stack2DRegister()
            << "@R" << tempIndex << "\n"
            << "M=D\n";
    }
}

void CodeWriter::writeConstant(ECommandType commandType, int index)
{
    if (commandType == ECommandType::C_PUSH)
    {
        if (index < 0 || index > SHRT_MAX)
        {
            throw std::runtime_error("Unsupported index " + std::to_string(index) + " with 'push constant' command.");
        }

        *output_file
            << "@" << index << "\n"
            << "D=A\n"
            << DRegister2Stack();
    }
    else if (commandType == ECommandType::C_POP)
    {
        throw std::runtime_error("Unsupported command 'pop constant " + std::to_string(index) + "'.");
    }
}

void CodeWriter::writeStatic(ECommandType commandType, int index)
{
    if (commandType == ECommandType::C_PUSH)
    {
        *output_file
            << "@Foo." << index << "\n"
            << "D=M\n"
            << DRegister2Stack();
    }
    else if (commandType == ECommandType::C_POP)
    {
        *output_file
            << stack2DRegister()
            << "@Foo." << index << "\n"
            << "M=D\n";
    }
}

std::string CodeWriter::stack2DRegister()
{
    return std::string(
R"(@SP
AM=M-1
D=M
)");
}

std::string CodeWriter::DRegister2Stack()
{
    return std::string(
R"(@SP
A=M
M=D
@SP
M=M+1
)");
}

int CodeWriter::getUniqNumber()
{
    return uniq_number++;
}