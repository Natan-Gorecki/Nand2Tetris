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
		
	}
	catch (const std::runtime_error& error)
	{
		std::cout << "Error: " << error.what() << "\n";
		return EXIT_FAILURE;
	}
}