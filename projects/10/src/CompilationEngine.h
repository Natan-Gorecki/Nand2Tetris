#pragma once
#include <functional>
#include <string>
#include "JackTokenizer.h"

/// <summary>
/// The CompilationEngine is the backbone module of both the syntax analyzer described in this chapter
///     and the full-scale compiler described in the next chapter.
/// <para/> In the syntax analyzer, the compliation engine emits a structured representation of the input source code
///     wrapped in XML tags.
/// <para/> In the compiler, the compilation engine will instead emit executable VM code.
/// </summary>
class CompilationEngine
{
public:
    /// <summary>
    /// Opens the output file and gets ready to write into it.
    /// <para/> The next routine called must be compileClass.
    /// </summary>
    /// <param name="filename">Name of the output file</param>
    /// <param name="jackTokenizer">Instance of Jack tokenizer</param>
    CompilationEngine(std::string filename, JackTokenizer* jackTokenizer);
    /// <summary>
    /// Closes the output file.
    /// </summary>
    ~CompilationEngine();

public:
    /// <summary>
    /// Compiles a complete class.
    /// </summary>
    void compileClass();

public:
    static void writeOutput(std::string text);
    static void writeToken(std::string text);
private:
    static std::function<void(std::string)> onWriteOutput;
    static std::function<void(std::string)> onWriteToken;

private:
    std::ofstream* mOutputFile = NULL;
    std::ofstream* mTokensFile = NULL;
    JackTokenizer* mJackTokenizer = NULL;

    bool mWriteTokens = true;
    std::string mOutputFileName = "";
    std::string mTokensFileName = "";
};
