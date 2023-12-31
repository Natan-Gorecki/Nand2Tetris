#pragma once

#include <string>

/// <summary>
/// The module provides services for translating symbolic Hack mnemonics into their binary codes.
/// </summary>
class CodeModule
{
public:
    /// <summary>
    /// Returns the binary code of the dest mnemonic.
    /// </summary>
    /// <returns>3 bits, as a string</returns>
    static std::string dest(const std::string& mnemonic);
    /// <summary>
    /// Returns the binary code of the comp mnemonic.
    /// </summary>
    /// <returns>7 bits, as a string</returns>
    static std::string comp(const std::string& mnemonic);
    /// <summary>
    /// Returns the binary code of the jump mnemonic.
    /// </summary>
    /// <returns>3 bits, as a string</returns>
    static std::string jump(const std::string& mnemonic);

private:
    /// <summary>
    /// Encapsulation to use only static methods
    /// </summary>
    CodeModule() = default;
};
