#pragma once
#include <string>

/// <summary>
/// JackAnalyzer is the main program that drives the overall syntax analysis process, using the services
///     of a JackTokenizer and a CompilationEngine.
/// </summary>
class JackCompiler
{
public:
    /// <summary>
    /// Validates provided path, creates JackTokenizer and CompilationEngine.
    /// May throw a runtime exception.
    /// </summary>
    explicit JackCompiler(std::string const& path);
    /// <summary>
    /// Releases allocated memory.
    /// </summary>
    ~JackCompiler() = default;

    /// <summary>
    /// Returns an information, whether provided path is directory or single file.
    /// </summary>
    bool isDirectoryPath() const;
    /// <summary>
    /// Parses all .jack files from specified directory.
    /// </summary>
    void parseDirectory() const;
    /// <summary>
    /// Parses specified .jack file.
    /// </summary>
    void parseSingleFile() const;

private:
    std::string mInputPath;
    bool mIsDirectoryPath;
};
