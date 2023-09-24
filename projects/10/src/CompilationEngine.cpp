#include "CompilationEngine.h"

/// <summary>
/// Opens the output file and gets ready to write into it.
/// <para/> The next routine called must be compileClass.
/// </summary>
/// <param name="filename">Name of the output file</param>
CompilationEngine::CompilationEngine(std::string filename)
{

}

/// <summary>
/// Closes the output file.
/// </summary>
CompilationEngine::~CompilationEngine()
{

}

/// <summary>
/// Compiles a complete class.
/// </summary>
void CompilationEngine::compileClass()
{

}

/// <summary>
/// Compiles a static variable declaration or a field declaration.
/// </summary>
void CompilationEngine::compileClassVarDec()
{

}

/// <summary>
/// Compiles a complete method, function or constructor.
/// </summary>
void CompilationEngine::compileSubroutine()
{

}

/// <summary>
/// Compiles a (possibly empty) parameter list. Does not handle the enclosing parentheses tokens ( and ).
/// </summary>
void CompilationEngine::compileParameterList()
{

}

/// <summary>
/// Compiles a subroutine's body.
/// </summary>
void CompilationEngine::compileSubroutineBody()
{

}

/// <summary>
/// Compiles a var declaration.
/// </summary>
void CompilationEngine::compileVarDec()
{

}

/// <summary>
/// Compiles a sequence of statements. Does not handle the enclosing curly bracket tokens { and }.
/// </summary>
void CompilationEngine::compileStatements()
{

}

/// <summary>
/// Compiles a let statement.
/// </summary>
void CompilationEngine::compileLet()
{

}

/// <summary>
/// Compiles an if statement, possibly with a trailing else clause.
/// </summary>
void CompilationEngine::compileIf()
{

}

/// <summary>
/// Compiles a while statement.
/// </summary>
void CompilationEngine::compileWhile()
{

}

/// <summary>
/// Compiles a do statement.
/// </summary>
void CompilationEngine::compileDo()
{

}

/// <summary>
/// Compiles a return statement.
/// </summary>
void CompilationEngine::compileReturn()
{

}

/// <summary>
/// Compiles an expression.
/// </summary>
void CompilationEngine::compileExpression()
{

}

/// <summary>
/// Compiles a term. If the current token is an identifier, the routine must resolve it
///		into a variable, an array element or a subroutine call.
/// A single lookahead token, which may be [, ( or . suffices to distinguish between the possibilities.
/// Any other token is not part of this term and should not be advanced over.
/// </summary>
void CompilationEngine::compileTerm()
{

}

/// <summary>
/// Compiles a (possibly empty) comma-separated list of expressions.
/// </summary>
/// <returns>Returns the number of expressions in the list</returns>
int CompilationEngine::compileExpressionList()
{

}
