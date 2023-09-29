#include <chrono>
#include<filesystem>
#include<iostream>
#include "JackAnalyzer.h"

int main(int argc, char* argv[])
{
	if (argc > 2)
	{
		std::cout << "Usage:\n"
			<< "\tJackAnalyzer InputDirectory\n"
			<< "\tJackAnalyzer InputFile.jack\n";
		return EXIT_FAILURE;
	}

	std::string path;
	if (argc == 1)
	{
		std::cout << "No file or directory specified. Searching for files in current directory.\n";
		path = std::filesystem::current_path().string();
	}
	else
	{
		path = argv[1];
	}

	try
	{
		JackAnalyzer jackAnalyzer = JackAnalyzer(path);

		auto startTime = std::chrono::high_resolution_clock::now();

		if (jackAnalyzer.isDirectoryPath())
		{
			jackAnalyzer.parseDirectory();
		}
		else
		{
			jackAnalyzer.parseSingleFile();
		}

		auto endTime = std::chrono::high_resolution_clock::now();

		std::cout << "Created tokens " << jackAnalyzer.getOutputFile() << " file in " << (endTime - startTime) / std::chrono::milliseconds(1) << " ms.\n";
		return EXIT_SUCCESS;
	}
	catch (const std::runtime_error& error)
	{
		std::cout << "Error: " << error.what() << "\n";
		return EXIT_FAILURE;
	}
}