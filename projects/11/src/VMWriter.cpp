#include "JackCompilerError.h"
#include "VMWriter.h"

using namespace std;

/// <summary>
/// Creates VMWriter.
/// </summary>
VMWriter::VMWriter(shared_ptr<ofstream> outputFile)
    : mOutputFile(outputFile)
{
}

/// <summary>
/// Writes a VM push command.
/// </summary>
void VMWriter::writePush(ESegment segment, int index)
{
    *mOutputFile << "push " << segmentToString(segment) << " " << index << "\n";
}

/// <summary>
/// Writes a VM pop command.
/// </summary>
void VMWriter::writePop(ESegment segment, int index)
{
    *mOutputFile << "pop " << segmentToString(segment) << " " << index << "\n";
}

/// <summary>
/// Writes a VM arithmetic-logical command.
/// </summary>
void VMWriter::writeArithmetic(EArithmetic command)
{
    *mOutputFile << arithmeticToString(command) << "\n";
}

/// <summary>
/// Writes a VM label command.
/// </summary>
void VMWriter::writeLabel(std::string label)
{
    *mOutputFile << "label " << label << "\n";
}

/// <summary>
/// Writes a VM goto command.
/// </summary>
void VMWriter::writeGoto(std::string label)
{
    *mOutputFile << "goto " << label << "\n";
}

/// <summary>
/// Writes a VM if-goto command.
/// </summary>
void VMWriter::writeIf(std::string label)
{
    *mOutputFile << "if-goto " << label << "\n";
}

/// <summary>
/// Writes a VM call command.
/// </summary>
void VMWriter::writeCall(std::string name, int nArgs)
{
    *mOutputFile << "call " << name << " " << nArgs << "\n";
}

/// <summary>
/// Writes a VM function command.
/// </summary>
void VMWriter::writeFunction(std::string name, int nArgs)
{
    *mOutputFile << "function " << name << " " << nArgs << "\n";
}

/// <summary>
/// Writes a VM return command.
/// </summary>
void VMWriter::writeReturn()
{
    *mOutputFile << "return\n";
}

string VMWriter::segmentToString(ESegment segment)
{
    switch (segment)
    {
    case ESegment::CONSTANT:
        return "constant";
    case ESegment::ARGUMENT:
        return "argument";
    case ESegment::LOCAL:
        return "local";
    case ESegment::STATIC:
        return "static";
    case ESegment::THIS:
        return "this";
    case ESegment::THAT:
        return "that";
    case ESegment::POINTER:
        return "pointer";
    case ESegment::TEMP:
        return "temp";
    case ESegment::UNDEFINED:
    default:
        throw JackCompilerError("Undefined segment.");
    }
}

string VMWriter::arithmeticToString(EArithmetic arithmetic)
{
    switch (arithmetic)
    {
    case EArithmetic::ADD:
        return "add";
    case EArithmetic::SUB:
        return "sub";
    case EArithmetic::NEG:
        return "neg";
    case EArithmetic::EQ:
        return "eq";
    case EArithmetic::GT:
        return "gt";
    case EArithmetic::LT:
        return "lt";
    case EArithmetic::AND:
        return "and";
    case EArithmetic::OR:
        return "or";
    case EArithmetic::NOT:
        return "not";
    case EArithmetic::UNDEFINED:
    default:
        throw JackCompilerError("Undefined arithmetic or logical command");
    }
}