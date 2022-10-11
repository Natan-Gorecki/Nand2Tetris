#pragma once
#include <fstream>
#include <string>
#include "ECommandType.h"

/// <summary>
/// The module translates a parsed VM command into Hack assembly code.
/// </summary>
class CodeWriter
{
public:
	/// <summary>
	/// Opens the output file and gets ready to write into it
	/// </summary>
	/// <param name="filename">Name of the output file</param>
	CodeWriter(std::string filename, bool generateComment);
	/// <summary>
	/// Closes the output file
	///</summary>
	~CodeWriter();

public:
	/// <summary>
	/// Writes to the output file the assembly code that implements the given arithmetic-logical command.
	/// </summary>
	/// <param name="command">Arithmetic-logical command</param>
	void writeArithmetic(std::string command);
	/// <summary>
	/// Writes to the output file the assembly code that implements the given push or pop command
	/// </summary>
	/// <param name="commandType">C_PUSH or C_POP</param>
	/// <param name="segment">Name of the segment</param>
	/// <param name="index">Numeric address</param>
	void writePushPop(ECommandType commandType, std::string segment, int index);

private:
	void initialCode();
	void finalCode();

	void writeComparisonCommand(std::string comparisonCheck);

	void writeLocalArgThisThat(ECommandType commandType, std::string segmentName, int index);
	void writePointer(ECommandType commandType, int index);
	void writeTemp(ECommandType commandType, int index);
	void writeConstant(ECommandType commandType, int index);
	void writeStatic(ECommandType commandType, int index);

	std::string stack2DRegister();
	std::string DRegister2Stack();
	int getUniqNumber();

private:
	std::ofstream* output_file = NULL;
	bool generate_comment = false;

	int uniq_number = 1;
	bool is_comparison_used = false;
};