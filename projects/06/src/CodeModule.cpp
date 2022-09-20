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
	{  "0", "101010"},
	{  "1", "111111"},
	{ "-1", "111010"},
	{  "D", "001100"},
	{  "A", "110000"}, {  "M", "110000"},
	{ "!D", "001101"},
	{ "!A", "110001"}, { "!M", "110001"},
	{ "-D", "001111"},
	{ "-A", "110011"}, { "-M", "110011"},
	{"D+1", "011111"},
	{"A+1", "110111"}, {"M+1", "110111"},
	{"D-1", "001110"},
	{"A-1", "110010"}, {"M-1", "110010"},
	{"D+A", "000010"}, {"D+M", "000010"},
	{"D-A", "010011"}, {"D-M", "010011"},
	{"A-D", "000111"}, {"M-D", "000111"},
	{"D&A", "000000"}, {"D&M", "000000"},
	{"D|A", "010101"}, {"D|M", "010101"}
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