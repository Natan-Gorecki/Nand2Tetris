#pragma once
#include <string>

#include "CodeWriter.h"

/// <summary>
/// VMTranslator contains main implementation to parse single file/ all files from the specified directory. It converts code from .vm files 
///		and creates single .asm file.
/// </summary>
class VMTranslator
{
public:
	/// <summary>
	/// Valides provided path, creates CodeWriter. 
	/// May throw a runtime exception.
	/// </summary>
	/// <param name="path"></param>
	VMTranslator(std::string path);
	/// <summary>
	/// Releases alocated memory.
	/// </summary>
	~VMTranslator();

public:
	/// <summary>
	/// Returns an information, whether provided path is directory or single file.
	/// </summary>
	bool isDirectoryPath();
	/// <summary>
	/// Parses all .vm files from specified directory.
	/// </summary>
	void parseDirectory();
	/// <summary>
	/// Parses specified .vm file.
	/// </summary>
	void parseSingleFile();

private:
	void parseSingleFile(std::string path);

private:
	std::string input_file;
	std::string output_file;
	bool is_directory_path;

	CodeWriter* code_writer = NULL;
};