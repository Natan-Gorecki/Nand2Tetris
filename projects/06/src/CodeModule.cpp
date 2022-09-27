#include "CodeModule.h"
#include <map>

std::map<std::string, std::string> dest_table = {
	{   "", "000"},
	{  "M", "001"},
	{  "D", "010"},
	{ "DM", "011"},
	{  "A", "100"},
	{ "AM", "101"},
	{ "AD", "110"},
	{"ADM", "111"}
};

std::map<std::string, std::string> comp_table = {
	{  "0", "0101010"},
	{  "1", "0111111"},
	{ "-1", "0111010"},
	{  "D", "0001100"},
	{  "A", "0110000"}, {  "M", "1110000"},
	{ "!D", "0001101"},
	{ "!A", "0110001"}, { "!M", "1110001"},
	{ "-D", "0001111"},
	{ "-A", "0110011"}, { "-M", "1110011"},
	{"D+1", "0011111"},
	{"A+1", "0110111"}, {"M+1", "1110111"},
	{"D-1", "0001110"},
	{"A-1", "0110010"}, {"M-1", "1110010"},
	{"D+A", "0000010"}, {"D+M", "1000010"},
	{"D-A", "0010011"}, {"D-M", "1010011"},
	{"A-D", "0000111"}, {"M-D", "1000111"},
	{"D&A", "0000000"}, {"D&M", "1000000"},
	{"D|A", "0010101"}, {"D|M", "1010101"}
};

std::map<std::string, std::string> jump_table = {
	{   "", "000"},
	{"JGT", "001"},
	{"JEQ", "010"},
	{"JGE", "011"},
	{"JLT", "100"},
	{"JNE", "101"},
	{"JLE", "110"},
	{"JMP", "111"}
};

/// <summary>
/// Returns the binary code of the dest mnemonic.
/// </summary>
/// <returns>3 bits, as a string</returns>
std::string CodeModule::dest(std::string mnemonic)
{
	std::map<std::string, std::string>::iterator it = dest_table.find(mnemonic);
	if (it != dest_table.end())
	{
		return it->second;
	}
	return "";
}

/// <summary>
/// Returns the binary code of the comp mnemonic.
/// </summary>
/// <returns>7 bits, as a string</returns>
std::string CodeModule::comp(std::string mnemonic)
{
	std::map<std::string, std::string>::iterator it = comp_table.find(mnemonic);
	if (it != comp_table.end())
	{
		return it->second;
	}
	return "";
}

/// <summary>
/// Returns the binary code of the jump mnemonic.
/// </summary>
/// <returns>3 bits, as a string</returns>
std::string CodeModule::jump(std::string mnemonic)
{
	std::map<std::string, std::string>::iterator it = jump_table.find(mnemonic);
	if (it != jump_table.end())
	{
		return it->second;
	}
	return "";
}