#include <regex>
#include <shlwapi.h>
#include "Parser.h"

const char* whitespace_chars = " \t\n\r\f\v";

// trim from end of string (right)
inline std::string& rtrim(std::string& s, const char* t = whitespace_chars)
{
	s.erase(s.find_last_not_of(t) + 1);
	return s;
}

// trim from beginning of string (left)
inline std::string& ltrim(std::string& s, const char* t = whitespace_chars)
{
	s.erase(0, s.find_first_not_of(t));
	return s;
}

// trim from both ends of string (right then left)
inline std::string& trim(std::string& s, const char* t = whitespace_chars)
{
	return ltrim(rtrim(s, t), t);
}


/// <summary>
/// Are there more lines in the input?
/// </summary>
bool Parser::hasMoreLines()
{
	return input_stream->peek() != EOF;
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
	current_line = "";
	instruction_type = InstructionType::UNDEFINED;
	f_symbol = "";
	f_dest = "";
	f_comp = "";
	f_jump = "";

	if (!std::getline(*input_stream, current_line))
		return;

	// remove comments
	auto comment_pos = current_line.find("//");
	if (comment_pos != std::string::npos)
	{
		current_line = comment_pos == 0 ? "" : current_line.substr(0, comment_pos);
	}
	current_line = trim(current_line);

	if (current_line == "")
	{
		instruction_type = InstructionType::COMMENT;
		return;
	}

	std::regex a_regex("@[\\w.$:]+"); // A_INSTRUCTION for @xxx
	std::regex c_regex("([ADM]{1,3}=)?([A-Z0-1-!+&|]{1,3})(;[A-Z]{1,3})?"); // C_INSTRUCTION for dest=comp;jump
	std::regex l_regex("\\([\\w.$:]+\\)"); // L_INSTRUCTION for (xxx)
	std::cmatch match;

	if (std::regex_match(current_line.c_str(), match, a_regex))
	{
		instruction_type = InstructionType::A_INSTRUCTION;
		line_number++;

		f_symbol = current_line.substr(1, current_line.length() - 1);
	}
	else if (std::regex_match(current_line.c_str(), match, c_regex))
	{
		instruction_type = InstructionType::C_INSTRUCTION;
		line_number++;
		
		auto equal_sign_pos = current_line.find("=");
		f_dest = equal_sign_pos != std::string::npos ? current_line.substr(0, equal_sign_pos) : "";
		
		auto semicolon_pos = current_line.find(";");
		f_jump = semicolon_pos != std::string::npos ? current_line.substr(semicolon_pos+1, current_line.length()-1-semicolon_pos) : "";
		
		auto comp_start = equal_sign_pos != std::string::npos ? equal_sign_pos + 1 : 0;
		auto comp_end = semicolon_pos != std::string::npos ? semicolon_pos - 1 : current_line.length()-1;
		f_comp = current_line.substr(comp_start, comp_end-comp_start+1);
	}
	else if (std::regex_match(current_line.c_str(), match, l_regex))
	{
		instruction_type = InstructionType::L_INSTRUCTION;
		f_symbol = current_line.substr(1, current_line.length() - 2);
	}
}

/// <summary>
/// Returns the current line address, where only A_INSTRUCTION and C_INSTRUCTION are counted
/// </summary>
int Parser::lineNumber()
{
	return line_number;
}

/// <summary>
/// Returns the type of the current instruction.
/// </summary>
InstructionType Parser::instructionType()
{
	return instruction_type;
}

/// <summary>
/// If the current instruction is (xxx), returns the symbol xxx.
/// If the current instruction is @xxx, returns the symbol or decimal xxx (as a string).
/// <para/> Should be called only if instructionType is A_INSTRUCTION or L_INSTRUCTION.
/// </summary>
std::string Parser::symbol()
{
	return f_symbol;
}

/// <summary>
/// Returns the symbolic dest part of the current C-instruction (8 possibilites).
/// <para/> Should be called only if instructionType is C_INSTRUCTION.
/// </summary>
std::string Parser::dest()
{
	return f_dest;
}
/// <summary>
/// Returns the symbolic jump part of the current C-instruction (28 possibilites).
/// <para/> Should be called only if instructionType is C_INSTRUCTION.
/// </summary>
std::string Parser::comp()
{
	return f_comp;
}

/// <summary>
/// Returns the symbolic jump part of the current C-instruction (8 possibilites).
/// <para/> Should be called only if instructionType is C_INSTRUCTION.
/// </summary>
std::string Parser::jump()
{
	return f_jump;
}

/// <summary>
/// Opens the input file/stream and gets ready to parse it.
/// </summary>
Parser::Parser(std::string filename)
{
	input_stream = new std::ifstream(filename);
	if (!input_stream->is_open())
	{
		throw std::runtime_error("Cannot open " + filename + " file");
	}
}

/// <summary>
/// Closes the input file/stream
/// </summary>
Parser::~Parser()
{
	if (input_stream)
	{
		input_stream->close();
		delete input_stream;
		input_stream = NULL;
	}
}