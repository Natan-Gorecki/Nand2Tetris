#pragma once
#include <string>
#include "InstructionType.h"

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
	Parser(std::ifstream inputStream);
};

