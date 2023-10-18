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
    /// <summary>
    /// Compiles a static variable declaration or a field declaration.
    /// </summary>
    void compileClassVarDec();
    /// <summary>
    /// Compiles a complete method, function or constructor.
    /// </summary>
    void compileSubroutine();
    /// <summary>
    /// Compiles a (possibly empty) parameter list. Does not handle the enclosing parentheses tokens ( and ).
    /// </summary>
    void compileParameterList();
    /// <summary>
    /// Compiles a subroutine's body.
    /// </summary>
    void compileSubroutineBody();
    /// <summary>
    /// Compiles a var declaration.
    /// </summary>
    void compileVarDec();
    /// <summary>
    /// Compiles a sequence of statements. Does not handle the enclosing curly bracket tokens { and }.
    /// </summary>
    void compileStatements();
    /// <summary>
    /// Compiles a let statement.
    /// </summary>
    void compileLet();
    /// <summary>
    /// Compiles an if statement, possibly with a trailing else clause.
    /// </summary>
    void compileIf();
    /// <summary>
    /// Compiles a while statement.
    /// </summary>
    void compileWhile();
    /// <summary>
    /// Compiles a do statement.
    /// </summary>
    void compileDo();
    /// <summary>
    /// Compiles a return statement.
    /// </summary>
    void compileReturn();
    /// <summary>
    /// Compiles an expression.
    /// </summary>
    void compileExpression();
    /// <summary>
    /// Compiles a term. If the current token is an identifier, the routine must resolve it
    ///     into a variable, an array element or a subroutine call.
    /// A single lookahead token, which may be [, ( or . suffices to distinguish between the possibilities.
    /// Any other token is not part of this term and should not be advanced over.
    /// </summary>
    void compileTerm();
    /// <summary>
    /// Compiles a (possibly empty) comma-separated list of expressions.
    /// </summary>
    /// <returns>Returns the number of expressions in the list</returns>
    int compileExpressionList();

public:
    static void writeOutput(std::string text);
    static void writeToken(std::string text);
    static void advanceToken();
private:
    static std::function<void(std::string)> onWriteOutput;
    static std::function<void(std::string)> onWriteToken;
    static std::function<void()> onAdvanceToken;

private:
    std::ofstream* mOutputFile = NULL;
    std::ofstream* mTokensFile = NULL;
    JackTokenizer* mJackTokenizer = NULL;

    bool mWriteTokens = true;
    std::string mTokensFileName = "";
};
