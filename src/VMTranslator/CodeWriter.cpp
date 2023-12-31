#include <iostream>
#include <regex>
#include "CodeWriter.h"
#include "VMTranslatorError.h"

using namespace std;

/// <summary>
/// Opens the output file and gets ready to write into it
/// </summary>
/// <param name="filename">Name of the output file</param>
/// <param name="generateComment">Information if comment should be generated</param>
CodeWriter::CodeWriter(const string& filename, bool generateComment)
    : mGenerateComment(generateComment)
{
    mOutputFile = make_unique<ofstream>(filename);
    if (!mOutputFile->is_open())
    {
        throw VMTranslatorError("Cannot find or open " + filename + " file.");
    }

    initialCode();
}

/// <summary>
/// Closes the output file
///</summary>
CodeWriter::~CodeWriter()
{
    finalCode();
}

/// <summary>
/// Informs that the translation of a new VM file has started (called by VMTranslator).
/// </summary>
/// <param name="fileName"></param>
void CodeWriter::setFileName(const string& fileName)
{
    mFileName = fileName;
}

/// <summary>
/// Writes to the output file the assembly code that implements the given arithmetic-logical command.
/// </summary>
/// <param name="command">Arithmetic-logical command</param>
void CodeWriter::writeArithmetic(const string& command)
{
    if (mGenerateComment)
    {
        *mOutputFile << "\n// " << command << "\n";
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
        *mOutputFile << addCommand;
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
        *mOutputFile << subCommand;
    }
    else if (command == "neg")
    {
        const char* negCommand =
R"(@SP
A=M-1
M=-M
)";
        *mOutputFile << negCommand;
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
        *mOutputFile << andCommand;
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
        *mOutputFile << orCommand;
    }
    else if (command == "not")
    {
        const char* notCommand =
R"(@SP
A=M-1
M=!M
)";
        *mOutputFile << notCommand;
    }
    else
    {
        throw VMTranslatorError("Unknown C_ARITHMETIC " + command + " command.");
    }    
}

/// <summary>
/// Writes to the output file the assembly code that implements the given push or pop command
/// </summary>
/// <param name="commandType">C_PUSH or C_POP</param>
/// <param name="segment">Name of the segment</param>
/// <param name="index">Numeric address</param>
void CodeWriter::writePushPop(ECommandType commandType, const string& segment, int index)
{
    string pushPop = commandType == ECommandType::C_PUSH ? "push" : "pop";
    if (mGenerateComment)
    {
        *mOutputFile << "\n// " << pushPop << " " << segment << " " << index << "\n";
    }
    
    if (commandType != ECommandType::C_PUSH && commandType != ECommandType::C_POP)
    {
        throw VMTranslatorError("Method only allowed with C_PUSH or C_POP command.");
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
        throw VMTranslatorError("Unknown " + segment + " segment.");
    }
}

/// <summary>
/// Writes assembly code that effects the label command
/// </summary>
/// <param name="label">Label name</param>
void CodeWriter::writeLabel(const string& label)
{
    if (mGenerateComment)
    {
        *mOutputFile << "\n// label " << label<< "\n";
    }

    regex labelRegex("[A-z.:][\\w.:]*");
    if (cmatch match; !regex_match(label.c_str(), match, labelRegex))
    {
        throw VMTranslatorError("Invalid label format: " + label);
    }

    auto fullName = this->getFullLabelName(label);

    if (mDefinedLabels.count(fullName))
    {
        throw VMTranslatorError("Duplicated labels " + fullName);
    }

    *mOutputFile
        << "(" << fullName << ")\n";

    mDefinedLabels.insert(fullName);
}

/// <summary>
/// Writes assembly code that effects the goto command.
/// </summary>
/// <param name="label">Label name</param>
void CodeWriter::writeGoto(const string& label)
{
    if (mGenerateComment)
    {
        *mOutputFile << "\n// goto " << label << "\n";
    }

    auto fullName = this->getFullLabelName(label);
    
    *mOutputFile
        << "@" << fullName << "\n"
        << "0;JMP\n";    

    mDefinedGoto.insert(fullName);
}

/// <summary>
/// Writes assembly code that effects the if-goto command.
/// </summary>
/// <param name="label">Label name</param>
void CodeWriter::writeIf(const string& label)
{
    if (mGenerateComment)
    {
        *mOutputFile << "\n// if-goto " << label << "\n";
    }

    auto fullName = this->getFullLabelName(label);

    *mOutputFile
        << stack2DRegister()
        << "@" << fullName << "\n"
        << "D;JNE\n";
    
    mDefinedGoto.insert(fullName);
}

/// <summary>
/// Writes assembly code that effects the function command.
/// </summary>
/// <param name="functionName">Function name</param>
/// <param name="nVars">Count of the function local variables</param>
void CodeWriter::writeFunction(const string& functionName, int nVars)
{
    if (mGenerateComment)
    {
        *mOutputFile << "\n// function " << functionName << " " << nVars << "\n";
    }

    this->setFunctionName(functionName);

    if (mDefinedLabels.count(functionName))
    {
        throw VMTranslatorError("Duplicated labels " + functionName);
    }

    *mOutputFile
        << "(" << functionName << ")\n";

    for (int i = 0; i < nVars; i++)
    {
        *mOutputFile
            << "@0\n"
            << "D=A\n"
            << DRegister2Stack();
    }

    mDefinedLabels.insert(functionName);
}

/// <summary>
/// Writes assembly code that effects the call command.
/// </summary>
/// <param name="functionName">Function name</param>
/// <param name="nArgs">Count of the arguments that have been pushed onto the stack before the call</param>
void CodeWriter::writeCall(const string& functionName, int nArgs)
{
    if (mGenerateComment)
    {
        *mOutputFile << "\n// call " << functionName << " " << nArgs << "\n";
    }

    auto returnLabel = functionName + "$ret." + std::to_string(getNumber(functionName));

    if (mDefinedLabels.count(returnLabel))
    {
        throw VMTranslatorError("Duplicated labels " + returnLabel);
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

    string callString(callCommand);
    callString = regex_replace(callString, regex("ReturnLabel"), returnLabel);
    callString = regex_replace(callString, regex("DRegister2Stack"), DRegister2Stack());
    callString = regex_replace(callString, regex("nArgs"), to_string(nArgs));
    callString = regex_replace(callString, regex("FunctionName"), functionName);
    
    *mOutputFile << callString;

    mDefinedGoto.insert(functionName);
}

/// <summary>
/// Writes assembly code that effects the return command.
/// </summary>
void CodeWriter::writeReturn()
{
    if (mGenerateComment)
    {
        *mOutputFile << "\n// return\n";
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

    string returnString(returnCommand);
    returnString = regex_replace(returnString, regex("stack2DRegister"), stack2DRegister());
    *mOutputFile << returnString;
}

/// <summary>
/// Validates if every label used in goto and if-goto is defined.
/// </summary>
void CodeWriter::validateGotoStatements()
{
    for (const auto& labelName : mDefinedGoto)
    {
        if (!mDefinedLabels.count(labelName))
        {
            throw VMTranslatorError("Label " + labelName + " is not defined.");
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
    *mOutputFile << initCode;
}

void CodeWriter::finalCode()
{
    const char* finalCommand =
R"(
(END)
@END
0;JMP
)";
    *mOutputFile << finalCommand;

    if (mDefinedLabels.count("Sys.init"))
    {
        *mOutputFile << "\n(CHECK_SYSINIT)\n";
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
        *mOutputFile << sysinitCheck;
    }

    if (mIsComparisonUsed)
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
        string comparisonString(comparisonCommand);
        comparisonString = regex_replace(comparisonString, regex("DREGISTER_2_STACK\n"), DRegister2Stack());
        *mOutputFile << comparisonString;
    }
}

void CodeWriter::writeComparisonCommand(const string& comparisonCheck)
{
    mIsComparisonUsed = true;

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
    string comparisonString(comparisonCommand);
    comparisonString = regex_replace(comparisonString, regex("AFTER_CONDITION"), string("AFTER_CONDITION.") + to_string(getNumber("AFTER_CONDITION")));
    comparisonString = regex_replace(comparisonString, regex("COMPARISON_CHECK"), comparisonCheck);
    *mOutputFile << comparisonString;
}

void CodeWriter::writeLocalArgThisThat(ECommandType commandType, const string& segmentName, int index)
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
        string pushString(pushCommand);
        pushString = regex_replace(pushString, regex("INDEX"), to_string(index));
        pushString = regex_replace(pushString, regex("SEGMENT_NAME"), segmentName);
        *mOutputFile 
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
        string popString(address2temp);
        popString = regex_replace(popString, regex("INDEX"), to_string(index));
        popString = regex_replace(popString, regex("SEGMENT_NAME"), segmentName);
        *mOutputFile 
            << popString 
            << stack2DRegister() 
            << value2Address;
    }
}

void CodeWriter::writePointer(ECommandType commandType, int index)
{
    if (index != 0 && index != 1)
    {
        throw VMTranslatorError("Unsupported index " + to_string(index) + " with pointer segment.");
    }

    const char* segmentName = index == 0 ? "THIS" : "THAT";

    if (commandType == ECommandType::C_PUSH)
    {
        *mOutputFile
            << "@" << segmentName << "\n"
            << "D=M\n"
            << DRegister2Stack();
    }
    else if (commandType == ECommandType::C_POP)
    {
        *mOutputFile
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
        throw VMTranslatorError("Unsupported index " + to_string(index) + " with temp segment.");
    }

    if (commandType == ECommandType::C_PUSH)
    {
        *mOutputFile
            << "@R" << tempIndex << "\n"
            << "D=M\n"
            << DRegister2Stack();
    }
    else if (commandType == ECommandType::C_POP)
    {
        *mOutputFile
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
            throw VMTranslatorError("Unsupported index " + to_string(index) + " with 'push constant' command.");
        }

        *mOutputFile
            << "@" << index << "\n"
            << "D=A\n"
            << DRegister2Stack();
    }
    else if (commandType == ECommandType::C_POP)
    {
        throw VMTranslatorError("Unsupported command 'pop constant " + to_string(index) + "'.");
    }
}

void CodeWriter::writeStatic(ECommandType commandType, int index)
{
    if (commandType == ECommandType::C_PUSH)
    {
        *mOutputFile
            << "@"<< mFileName << "." << index << "\n"
            << "D=M\n"
            << DRegister2Stack();
    }
    else if (commandType == ECommandType::C_POP)
    {
        *mOutputFile
            << stack2DRegister()
            << "@" << mFileName << "." << index << "\n"
            << "M=D\n";
    }
}

void CodeWriter::setFunctionName(const string& functionName)
{
    regex functionRegex("[A-z][\\w.]*");
    if (cmatch match; !regex_match(functionName.c_str(), match, functionRegex))
    {
        throw VMTranslatorError("Invalid function name format: " + functionName);
    }

    mFunctionName = functionName;
    mNoFunctionDefined = false;
}

string CodeWriter::getFullLabelName(const string& label)
{
    if (mNoFunctionDefined)
    {
        return mDefaultFunctionName + "$" + label;
    }

    if (mFunctionName.empty())
    {
        throw VMTranslatorError("Function name is empty.");
    }

    return mFunctionName + "$" + label;
}

string CodeWriter::stack2DRegister() const
{
    return string(
R"(@SP
AM=M-1
D=M
)");
}

string CodeWriter::DRegister2Stack() const
{
    return string(
R"(@SP
A=M
M=D
@SP
M=M+1
)");
}

int CodeWriter::getNumber(const string& label)
{
    auto it = mCountersMap.find(label);
    
    int newValue = 1;
    if (it != mCountersMap.end())
    {
        newValue = ++it->second;
    }
    
    mCountersMap.insert_or_assign(label, newValue);
    return newValue;
}
