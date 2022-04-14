#pragma once
#include <string>

class CodeModule
{
public:
	/// <summary>
	/// Returns the binary code of the dest mnemonic.
	/// </summary>
	/// <returns>3 bits, as a string</returns>
	std::string dest(std::string mnemonic);
	/// <summary>
	/// Returns the binary code of the comp mnemonic.
	/// </summary>
	/// <returns>7 bits, as a string</returns>
	std::string comp(std::string mnemonic);
	/// <summary>
	/// Returns the binary code of the jump mnemonic.
	/// </summary>
	/// <returns>3 bits, as a string</returns>
	std::string jump(std::string mnemonic);
};

