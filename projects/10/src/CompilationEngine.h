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
    /// Creates CompilationEngine.
    /// </summary>
    /// <param name="filename">Name of the output file</param>
    /// <param name="jackTokenizer">Instance of Jack tokenizer</param>
    CompilationEngine(const std::string& filename, std::shared_ptr<JackTokenizer> jackTokenizer);

    /// <summary>
    /// Compiles a complete class.
    /// </summary>
    void compileClass();

    static void writeOutput(std::string& text);
    static void writeToken(std::string& text);

private:
    void beforeCompile();
    void afterCompile();

    static std::function<void(std::string&)> onWriteOutput;
    static std::function<void(std::string&)> onWriteToken;

    std::unique_ptr<std::ofstream> mOutputFile;
    std::unique_ptr<std::ofstream> mTokensFile;
    std::shared_ptr<JackTokenizer> mJackTokenizer;

    bool mWriteTokens = true;
    std::string mOutputFileName = "";
    std::string mTokensFileName = "";
};
