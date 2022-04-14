#pragma once

enum class InstructionType
{
	/// <summary>
	/// A_INSTRUCTION for @xxx, where xxx is either a decimal number or a symbol.
	/// </summary>
	A_INSTRUCTION = 0,
	/// <summary>
	/// C_INSTRUCTION for dest=comp;jump
	/// </summary>
	C_INSTRUCTION = 1,
	/// <summary>
	/// L_INSTRUCTION for (xxx), where xxx is a symbol.
	/// </summary>
	L_INSTRUCTION = 2
};