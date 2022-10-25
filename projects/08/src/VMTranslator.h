#pragma once
#include <string>

class VMTranslator
{
public:
	VMTranslator(std::string path);
	~VMTranslator();

public:
	bool isDirectoryPath();
	void parseDirectory();
	void parseSingleFile();

private:
	std::string input_file;
	std::string output_file;
	bool is_directory_path;
};