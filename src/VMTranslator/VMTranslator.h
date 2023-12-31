#pragma once
#include <string>
#include "CodeWriter.h"

/// <summary>
/// VMTranslator contains main implementation to parse single file or all files from the specified directory. 
///     It converts code from .vm files and creates single .asm file.
/// </summary>
class VMTranslator
{
public:
    /// <summary>
    /// Valides provided path, creates CodeWriter.
    /// May throw a runtime exception.
    /// </summary>
    /// <param name="path"></param>
    explicit VMTranslator(const std::string& path);

    /// <summary>
    /// Returns an information, whether provided path is directory or single file.
    /// </summary>
    bool isDirectoryPath() const;
    /// <summary>
    /// Parses all .vm files from specified directory.
    /// </summary>
    void parseDirectory();
    /// <summary>
    /// Parses specified .vm file.
    /// </summary>
    void parseSingleFile();
    /// <summary>
    /// Gets output file name.
    /// </summary>
    /// <returns></returns>
    std::string getOutputFile() const;

private:
    void parseSingleFile(const std::string& path);

    std::string mInputFileName;
    std::string mOutputFileName;
    bool mIsDirectoryPath;

    std::unique_ptr<CodeWriter> mCodeWriter;
};
