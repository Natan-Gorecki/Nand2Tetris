#include <regex>
#include "HackAssemblerError.h"
#include "Parser.h"

using namespace std;

const char* const whitespace_chars = " \t\n\r\f\v";

// trim from end of string (right)
inline string& rtrim(string& s, const char* t = whitespace_chars)
{
    s.erase(s.find_last_not_of(t) + 1);
    return s;
}

// trim from beginning of string (left)
inline string& ltrim(string& s, const char* t = whitespace_chars)
{
    s.erase(0, s.find_first_not_of(t));
    return s;
}

// trim from both ends of string (right then left)
inline string& trim(string& s, const char* t = whitespace_chars)
{
    return ltrim(rtrim(s, t), t);
}


/// <summary>
/// Are there more lines in the input?
/// </summary>
bool Parser::hasMoreLines()
{
    return mInputStream->peek() != EOF;
}

/// <summary>
/// Skips over white space and comments, if necessary.
/// <para/> Reads the next instruction from the input, and makes it the current instruction.
/// <para/> The routine should be called only if hasMoreLines is true.
/// <para/> Initially there is no current instruction.
/// </summary>
void Parser::advance()
{
    // initialize fields
    mCurrentLine = "";
    mInstructionType = InstructionType::UNDEFINED;
    mSymbol = "";
    mDest = "";
    mComp = "";
    mJump = "";

    if (!getline(*mInputStream, mCurrentLine))
        return;

    // remove comments
    if (auto commentPos = mCurrentLine.find("//"); commentPos != std::string::npos)
    {
        mCurrentLine = commentPos == 0 ? "" : mCurrentLine.substr(0, commentPos);
    }
    mCurrentLine = trim(mCurrentLine);

    if (mCurrentLine == "")
    {
        mInstructionType = InstructionType::COMMENT;
        return;
    }

    std::regex a_regex("@[\\w.$:]+"); // A_INSTRUCTION for @xxx
    std::regex c_regex("([ADM]{1,3}=)?([A-Z0-1-!+&|]{1,3})(;[A-Z]{1,3})?"); // C_INSTRUCTION for dest=comp;jump
    std::regex l_regex("\\([\\w.$:]+\\)"); // L_INSTRUCTION for (xxx)
    std::cmatch match;

    if (std::regex_match(mCurrentLine.c_str(), match, a_regex))
    {
        mInstructionType = InstructionType::A_INSTRUCTION;
        mLineNumber++;

        mSymbol = mCurrentLine.substr(1, mCurrentLine.length() - 1);
    }
    else if (std::regex_match(mCurrentLine.c_str(), match, c_regex))
    {
        mInstructionType = InstructionType::C_INSTRUCTION;
        mLineNumber++;
        
        auto equal_sign_pos = mCurrentLine.find("=");
        mDest = equal_sign_pos != std::string::npos ? mCurrentLine.substr(0, equal_sign_pos) : "";
        
        auto semicolon_pos = mCurrentLine.find(";");
        mJump = semicolon_pos != std::string::npos ? mCurrentLine.substr(semicolon_pos + 1, mCurrentLine.length() - 1 - semicolon_pos) : "";
        
        auto comp_start = equal_sign_pos != std::string::npos ? equal_sign_pos + 1 : 0;
        auto comp_end = semicolon_pos != std::string::npos ? semicolon_pos - 1 : mCurrentLine.length() - 1;
        mComp = mCurrentLine.substr(comp_start, comp_end-comp_start+1);
    }
    else if (std::regex_match(mCurrentLine.c_str(), match, l_regex))
    {
        mInstructionType = InstructionType::L_INSTRUCTION;
        mSymbol = mCurrentLine.substr(1, mCurrentLine.length() - 2);
    }
}

/// <summary>
/// Returns the current line address, where only A_INSTRUCTION and C_INSTRUCTION are counted
/// </summary>
int Parser::lineNumber() const
{
    return mLineNumber;
}

/// <summary>
/// Returns the type of the current instruction.
/// </summary>
InstructionType Parser::instructionType() const
{
    return mInstructionType;
}

/// <summary>
/// If the current instruction is (xxx), returns the symbol xxx.
/// If the current instruction is @xxx, returns the symbol or decimal xxx (as a string).
/// <para/> Should be called only if instructionType is A_INSTRUCTION or L_INSTRUCTION.
/// </summary>
string Parser::symbol() const
{
    return mSymbol;
}

/// <summary>
/// Returns the symbolic dest part of the current C-instruction (8 possibilites).
/// <para/> Should be called only if instructionType is C_INSTRUCTION.
/// </summary>
string Parser::dest() const
{
    return mDest;
}
/// <summary>
/// Returns the symbolic jump part of the current C-instruction (28 possibilites).
/// <para/> Should be called only if instructionType is C_INSTRUCTION.
/// </summary>
string Parser::comp() const
{
    return mComp;
}

/// <summary>
/// Returns the symbolic jump part of the current C-instruction (8 possibilites).
/// <para/> Should be called only if instructionType is C_INSTRUCTION.
/// </summary>
string Parser::jump() const
{
    return mJump;
}

/// <summary>
/// Opens the input file/stream and gets ready to parse it.
/// </summary>
Parser::Parser(const string& filename)
{
    mInputStream = make_unique<ifstream>(filename);
    if (!mInputStream->is_open())
    {
        throw HackAssemblerError("Cannot open " + filename + " file");
    }
}
