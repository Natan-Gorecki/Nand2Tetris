#pragma once
#include <functional>
#include <string>
#include "JackTokenizer.h"
#include "VMWriter.h"

/// <summary>
/// The CompilationEngine is the backbone module of both the syntax analyzer and the full-scale compiler.
/// <para/> In the syntax analyzer, the compliation engine emits a structured representation of the input source code wrapped in XML tags.
/// <para/> In the compiler, the compilation engine will instead emit executable VM code.
/// </summary>
class CompilationEngine
{
public:
    /// <summary>
    /// Creates CompilationEngine.
    /// </summary>
    /// <param name="filename">Name of the input file</param>
    /// <param name="jackTokenizer">Instance of Jack tokenizer</param>
    explicit CompilationEngine(const std::string& filename);

    /// <summary>
    /// Compiles a single file.
    /// </summary>
    void compileFile();

private:
    void beforeCompile();
    void afterCompile();

    bool mWriteXmlSyntax = true;
    bool mWriteXmlTokens = true;
    bool mWriteVMCode = true;

    std::string mInputFileName = "";
    std::string mXmlSyntaxFileName = "";
    std::string mXmlTokensFileName = "";
    std::string mVMCodeFileName = "";

    std::unique_ptr<std::ofstream> mXmlSyntaxFile;
    std::unique_ptr<std::ofstream> mXmlTokensFile;
    std::shared_ptr<std::ofstream> mVMCodeFile;
    std::unique_ptr<VMWriter> mVMWriter;
};
