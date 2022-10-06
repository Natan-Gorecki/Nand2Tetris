#include "CodeWriter.h"

/// <summary>
/// Writes to the output file the assembly code that implements the given arithmetic-logical command.
/// </summary>
/// <param name="command">Arithmetic-logical command</param>
void CodeWriter::writeArithmetic(std::string command)
{

}

/// <summary>
/// Writes to the output file the assembly code that implements the given push or pop command
/// </summary>
/// <param name="commandType">C_PUSH or C_POP</param>
/// <param name="segment">Name of the segment</param>
/// <param name="index">Numeric address</param>
void CodeWriter::writePushPop(ECommandType commandType, std::string segment, int index)
{

}

/// <summary>
/// Opens the output file and gets ready to write into it
/// </summary>
/// <param name="filename">Name of the output file</param>
CodeWriter::CodeWriter(std::string filename)
{

}

/// <summary>
/// Closes the output file
///</summary>
CodeWriter::~CodeWriter()
{

}
