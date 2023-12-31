#include <map>
#include <stdexcept>
#include "CodeModule.h"

using namespace std;

const map<string, string, less<>> dest_table = {
    {   "", "000"},
    {  "M", "001"},
    {  "D", "010"},
    { "DM", "011"}, { "MD", "011"},
    {  "A", "100"},
    { "AM", "101"}, { "MA", "101"},
    { "AD", "110"}, { "DA", "110"},
    {"ADM", "111"}, {"AMD", "111"}, {"DAM", "111"}, {"DMA", "111"}, {"MAD", "111"}, {"MDA", "111"}
};

const map<string, string, less<>> comp_table = {
    {  "0", "0101010"},
    {  "1", "0111111"},
    { "-1", "0111010"},
    {  "D", "0001100"},
    {  "A", "0110000"}, {  "M", "1110000"},
    { "!D", "0001101"},
    { "!A", "0110001"}, { "!M", "1110001"},
    { "-D", "0001111"},
    { "-A", "0110011"}, { "-M", "1110011"},
    {"D+1", "0011111"}, {"1+D", "0011111"},
    {"A+1", "0110111"}, {"1+A", "0110111"}, {"M+1", "1110111"}, {"1+M", "1110111"},
    {"D-1", "0001110"},
    {"A-1", "0110010"}, {"M-1", "1110010"},
    {"D+A", "0000010"}, {"A+D", "0000010"}, {"D+M", "1000010"}, {"M+D", "1000010"},
    {"D-A", "0010011"}, {"D-M", "1010011"},
    {"A-D", "0000111"}, {"M-D", "1000111"},
    {"D&A", "0000000"}, {"A&D", "0000000"}, {"D&M", "1000000"}, {"M&D", "1000000"},
    {"D|A", "0010101"}, {"A|D", "0010101"}, {"D|M", "1010101"}, {"M|D", "1010101"}
};

const map<string, string, less<>> jump_table = {
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
string CodeModule::dest(const string& mnemonic)
{
    if (auto it = dest_table.find(mnemonic); it != dest_table.end())
    {
        return it->second;
    }
    throw runtime_error("Error: Failed to convert dest mnemonic '" + mnemonic + "' to binary.");
}

/// <summary>
/// Returns the binary code of the comp mnemonic.
/// </summary>
/// <returns>7 bits, as a string</returns>
string CodeModule::comp(const string& mnemonic)
{
    if (auto it = comp_table.find(mnemonic); it == comp_table.end())
    {
        return it->second;
    }
    throw runtime_error("Error: Failed to convert comp mnemonic '" + mnemonic + "' to binary.");
}

/// <summary>
/// Returns the binary code of the jump mnemonic.
/// </summary>
/// <returns>3 bits, as a string</returns>
string CodeModule::jump(const string& mnemonic)
{
    if (auto it = jump_table.find(mnemonic); it != jump_table.end())
    {
        return it->second;
    }
    throw runtime_error("Error: Failed to convert jump mnemonic " + mnemonic + " to binary.");
}
