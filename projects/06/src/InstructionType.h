#pragma once

enum class InstructionType
{
	/// <summary>
	/// Undefined
	/// </summary>
	UNDEFINED = 0,
	/// <summary>
	/// A_INSTRUCTION for @xxx, where xxx is either a decimal number or a symbol.
	/// </summary>
	A_INSTRUCTION = 1,
	/// <summary>
	/// C_INSTRUCTION for dest=comp;jump
	/// </summary>
	C_INSTRUCTION = 2,
	/// <summary>
	/// L_INSTRUCTION for (xxx), where xxx is a symbol.
	/// </summary>
	L_INSTRUCTION = 3
};