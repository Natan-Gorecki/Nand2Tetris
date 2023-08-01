#pragma once
#include <fstream>
#include <set>
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
	/// Informs that the translation of a new VM file has started (called by VMTranslator).
	/// </summary>
	/// <param name="fileName">File name</param>
	void setFileName(std::string fileName);
	/// <summary>
	/// Informs that the translation of a new function has started (called by VMTranslator).
	/// </summary>
	/// <param name="functionName">Function name</param>
	void setFunctionName(std::string functionName);
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
	/// <summary>
	/// Writes assembly code that effects the label command
	/// </summary>
	/// <param name="label">Label name</param>
	void writeLabel(std::string label);
	/// <summary>
	/// Writes assembly code that effects the goto command.
	/// </summary>
	/// <param name="label">Label name</param>
	void writeGoto(std::string label);
	/// <summary>
	/// Writes assembly code that effects the if-goto command.
	/// </summary>
	/// <param name="label">Label name</param>
	void writeIf(std::string label);
	/// <summary>
	/// Writes assembly code that effects the function command.
	/// </summary>
	/// <param name="functionName">Function name</param>
	/// <param name="nVars">Count of the function local variables</param>
	void writeFunction(std::string functionName, int nVars);
	/// <summary>
	/// Writes assembly code that effects the call command.
	/// </summary>
	/// <param name="functionName">Function name</param>
	/// <param name="nArgs">Count of the arguments that have been pushed onto the stack before the call</param>
	void writeCall(std::string functionName, int nArgs);
	/// <summary>
	/// Writes assembly code that effects the return command.
	/// </summary>
	void writeReturn();
	/// <summary>
	/// Validates if every label used in goto and if-goto is defined.
	/// </summary>
	void validateGotoStatements();

private:
	void initialCode();
	void finalCode();

	void writeComparisonCommand(std::string comparisonCheck);

	void writeLocalArgThisThat(ECommandType commandType, std::string segmentName, int index);
	void writePointer(ECommandType commandType, int index);
	void writeTemp(ECommandType commandType, int index);
	void writeConstant(ECommandType commandType, int index);
	void writeStatic(ECommandType commandType, int index);

	std::string getFullLabelName(std::string label);
	std::string getFullFunctionName(std::string functionName);

	std::string stack2DRegister();
	std::string DRegister2Stack();
	int getUniqNumber();

private:
	std::ofstream* output_file = NULL;
	bool generate_comment = false;

	int uniq_number = 1;
	bool is_comparison_used = false;

	std::set<std::string> defined_labels = std::set<std::string>();
	std::set<std::string> defined_goto = std::set<std::string>();
	
	std::string file_name = "";
	std::string function_name = "";
	std::string default_function_name = "Main";
	bool no_function_defined = true;
};