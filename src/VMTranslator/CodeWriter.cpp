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
/// Informs that the translation of a new VM file has started (called by VMTranslator).
/// </summary>
/// <param name="fileName"></param>
void CodeWriter::setFileName(std::string fileName)
{
    this->file_name = fileName;
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

/// <summary>
/// Writes assembly code that effects the label command
/// </summary>
/// <param name="label">Label name</param>
void CodeWriter::writeLabel(std::string label)
{
    if (generate_comment)
    {
        *output_file << "\n// label " << label<< "\n";
    }

    std::regex labelRegex("[A-z.:][\\w.:]*");
    std::cmatch match;
    if (!std::regex_match(label.c_str(), match, labelRegex))
    {
        throw std::runtime_error("Invalid label format: " + label);
    }

    std::string fullName = this->getFullLabelName(label);

    if (defined_labels.count(fullName))
    {
        throw std::runtime_error("Duplicated labels " + fullName);
    }

    *output_file
        << "(" << fullName << ")\n";

    defined_labels.insert(fullName);
}

/// <summary>
/// Writes assembly code that effects the goto command.
/// </summary>
/// <param name="label">Label name</param>
void CodeWriter::writeGoto(std::string label)
{
    if (generate_comment)
    {
        *output_file << "\n// goto " << label << "\n";
    }

    std::string fullName = this->getFullLabelName(label);
    
    *output_file
        << "@" << fullName << "\n"
        << "0;JMP\n";    

    defined_goto.insert(fullName);
}

/// <summary>
/// Writes assembly code that effects the if-goto command.
/// </summary>
/// <param name="label">Label name</param>
void CodeWriter::writeIf(std::string label)
{
    if (generate_comment)
    {
        *output_file << "\n// if-goto " << label << "\n";
    }

    std::string fullName = this->getFullLabelName(label);

    *output_file
        << stack2DRegister()
        << "@" << fullName << "\n"
        << "D;JNE\n";
    
    defined_goto.insert(fullName);
}

/// <summary>
/// Writes assembly code that effects the function command.
/// </summary>
/// <param name="functionName">Function name</param>
/// <param name="nVars">Count of the function local variables</param>
void CodeWriter::writeFunction(std::string functionName, int nVars)
{
    if (generate_comment)
    {
        *output_file << "\n// function " << functionName << " " << nVars << "\n";
    }

    this->setFunctionName(functionName);

    if (defined_labels.count(functionName))
    {
        throw std::runtime_error("Duplicated labels " + functionName);
    }

    *output_file
        << "(" << functionName << ")\n";

    for (int i = 0; i < nVars; i++)
    {
        *output_file
            << "@0\n"
            << "D=A\n"
            << DRegister2Stack();
    }

    defined_labels.insert(functionName);
}

/// <summary>
/// Writes assembly code that effects the call command.
/// </summary>
/// <param name="functionName">Function name</param>
/// <param name="nArgs">Count of the arguments that have been pushed onto the stack before the call</param>
void CodeWriter::writeCall(std::string functionName, int nArgs)
{
    if (generate_comment)
    {
        *output_file << "\n// call " << functionName << " " << nArgs << "\n";
    }

    std::string returnLabel = functionName + "$ret." + std::to_string(getNumber(functionName));

    if (defined_labels.count(returnLabel))
    {
        throw std::runtime_error("Duplicated labels " + returnLabel);
    }

    const char* callCommand =
R"(@ReturnLabel
D=A
DRegister2Stack
@LCL
D=M
DRegister2Stack
@ARG
D=M
DRegister2Stack
@THIS
D=M
DRegister2Stack
@THAT
D=M
DRegister2Stack
@nArgs
D=A
@5
D=A+D
@SP
D=M-D
@ARG
M=D
@SP
D=M
@LCL
M=D
@FunctionName
0;JMP
(ReturnLabel)
)";

    std::string callString(callCommand);
    callString = std::regex_replace(callString, std::regex("ReturnLabel"), returnLabel);
    callString = std::regex_replace(callString, std::regex("DRegister2Stack"), DRegister2Stack());
    callString = std::regex_replace(callString, std::regex("nArgs"), std::to_string(nArgs));
    callString = std::regex_replace(callString, std::regex("FunctionName"), functionName);
    
    *output_file << callString;

    defined_goto.insert(functionName);
}

/// <summary>
/// Writes assembly code that effects the return command.
/// </summary>
void CodeWriter::writeReturn()
{
    if (generate_comment)
    {
        *output_file << "\n// return\n";
    }

    const char* returnCommand =
R"(@LCL
D=M
@R13
M=D

@5
D=A
@R13
A=M-D
D=M
@R14
M=D

stack2DRegister
@ARG
A=M
M=D

@ARG
D=M
@SP
M=D+1

@R13
AM=M-1
D=M
@THAT
M=D
@R13
AM=M-1
D=M
@THIS
M=D
@R13
AM=M-1
D=M
@ARG
M=D
@R13
AM=M-1
D=M
@LCL
M=D

@R14
A=M
0;JMP
)";

    std::string returnString(returnCommand);
    returnString = std::regex_replace(returnString, std::regex("stack2DRegister"), stack2DRegister());
    *output_file << returnString;
}

/// <summary>
/// Validates if every label used in goto and if-goto is defined.
/// </summary>
void CodeWriter::validateGotoStatements()
{
    for (std::string labelName : defined_goto)
    {
        if (!defined_labels.count(labelName))
        {
            throw std::runtime_error("Label " + labelName + " is not defined.");
        }
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
@CHECK_SYSINIT
0;JMP
(NO_SYSINIT)
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

    if (defined_labels.count("Sys.init"))
    {
        *output_file << "\n(CHECK_SYSINIT)\n";
        writeCall("Sys.init", 0);
    }
    else
    {
        const char* sysinitCheck =
R"(
(CHECK_SYSINIT)
@NO_SYSINIT
0;JMP
)";
        *output_file << sysinitCheck;
    }

    if (is_comparison_used)
    {
        const char* comparisonCommand =
R"(
(TRUE_CONDITION)
@1
D=-A
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
R"(@AFTER_CONDITION
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
(AFTER_CONDITION)
)";
    std::string comparisonString(comparisonCommand);
    comparisonString = std::regex_replace(comparisonString, std::regex("AFTER_CONDITION"), std::string("AFTER_CONDITION.") + std::to_string(getNumber("AFTER_CONDITION")));
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
@R13
M=D
)";
        const char* value2Address =
R"(@R13
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
            << "@"<< file_name << "." << index << "\n"
            << "D=M\n"
            << DRegister2Stack();
    }
    else if (commandType == ECommandType::C_POP)
    {
        *output_file
            << stack2DRegister()
            << "@" << file_name << "." << index << "\n"
            << "M=D\n";
    }
}

void CodeWriter::setFunctionName(std::string functionName)
{
    std::regex functionRegex("[A-z][\\w.]*");
    std::cmatch match;
    if (!std::regex_match(functionName.c_str(), match, functionRegex))
    {
        throw std::runtime_error("Invalid function name format: " + functionName);
    }

    this->function_name = functionName;
    this->no_function_defined = false;
}

std::string CodeWriter::getFullLabelName(std::string label)
{
    if (this->no_function_defined)
    {
        return default_function_name + "$" + label;
    }

    if (function_name.empty())
    {
        throw std::runtime_error("Function name is empty.");
    }

    return function_name + "$" + label;
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

int CodeWriter::getNumber(std::string label)
{
    std::unordered_map<std::string, int>::iterator it = counters_map.find(label);
    
    int newValue = 1;
    if (it != counters_map.end())
    {
        newValue = ++it->second;    
    }
    
    counters_map.insert_or_assign(label, newValue);
    return newValue;
}
