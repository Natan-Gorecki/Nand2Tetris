#pragma once
#include <fstream>
#include <string>
#include "InstructionType.h"

/// <summary>
/// The Parser encapsulates access to the input assembly code. In particular, it provides a convenient means for advancing through the source code,
/// skipping comments and white space, and breaking each symbolic instruction into its underlying components.
/// </summary>
class Parser
{
public:
	/// <summary>
	/// Are there more lines in the input?
	/// </summary>
	bool hasMoreLines();
	/// <summary>
	/// Skips over white space and comments, if necessary.
	/// <para/> Reads the next instruction from the input, and makes it the current instruction.
	/// <para/> The routine should be called only if hasMoreLines is true.
	/// <para/> Initially there is no current instruction.
	/// </summary>
	void advance();
	/// <summary>
	/// Returns the current line address, where only A_INSTRUCTION and C_INSTRUCTION are counted
	/// </summary>
	int lineNumber();
	/// <summary>
	/// Returns the type of the current instruction.
	/// </summary>
	InstructionType instructionType();
	/// <summary>
	/// If the current instruction is (xxx), returns the symbol xxx.
	/// If the current instruction is @xxx, returns the symbol or decimal xxx (as a string).
	/// <para/> Should be called only if instructionType is A_INSTRUCTION or L_INSTRUCTION.
	/// </summary>
	std::string symbol();
	/// <summary>
	/// Returns the symbolic dest part of the current C-instruction (8 possibilites).
	/// <para/> Should be called only if instructionType is C_INSTRUCTION.
	/// </summary>
	std::string dest();
	/// <summary>
	/// Returns the symbolic jump part of the current C-instruction (28 possibilites).
	/// <para/> Should be called only if instructionType is C_INSTRUCTION.
	/// </summary>
	std::string comp();
	/// <summary>
	/// Returns the symbolic jump part of the current C-instruction (8 possibilites).
	/// <para/> Should be called only if instructionType is C_INSTRUCTION.
	/// </summary>
	std::string jump();

public:
	/// <summary>
	/// Opens the input file/stream and gets ready to parse it.
	/// </summary>
	Parser(std::string filename);

	/// <summary>
	/// Closes the input file/stream
	/// </summary>
	~Parser();

private:
	std::ifstream* input_stream = NULL;
	std::string current_line = "";
	int line_number = -1;

	InstructionType instruction_type = InstructionType::UNDEFINED;

	std::string f_symbol = "";
	std::string f_dest= "";
	std::string f_comp = "";
	std::string f_jump = "";
};

