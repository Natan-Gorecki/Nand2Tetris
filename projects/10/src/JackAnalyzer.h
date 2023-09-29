#pragma once
#include <string>

/// <summary>
/// JackAnalyzer is the main program that drives the overall syntax analysis process, using the services
///		of a JackTokenizer and a CompilationEngine.
/// </summary>
class JackAnalyzer
{
public:
	/// <summary>
	/// Validates provided path, creates JackTokenizer and CompilationEngine.
	/// May throw a runtime exception.
	/// </summary>
	JackAnalyzer(std::string path);
	/// <summary>
	/// Releases allocated memory.
	/// </summary>
	~JackAnalyzer();

public:
	/// <summary>
	/// Returns an information, whether provided path is directory or single file.
	/// </summary>
	bool isDirectoryPath();
	/// <summary>
	/// Parses all .jack files from specified directory.
	/// </summary>
	void parseDirectory();
	/// <summary>
	/// Parses specified .jack file.
	/// </summary>
	void parseSingleFile();
	/// <summary>
	/// Gets output file name.
	/// </summary>
	/// <returns></returns>
	std::string getOutputFile();

private:
	void parseSingleFile(std::string path);

private:
	std::string mInputFile;
	std::string mOutputFile;
	bool mIsDirectoryPath;
};
