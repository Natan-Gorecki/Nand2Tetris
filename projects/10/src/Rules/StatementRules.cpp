#include "ExpressionRules.h"
#include "LexicalRules.h"
#include "ProgramStructureRules.h"
#include "StatementRules.h"
#include "../CompilationEngine.h"

#pragma region StatementsRule
StatementsRule::StatementsRule() : ZeroOrMoreRule([]
    {
        return new StatementRule;
    })
{
}

void StatementsRule::compile()
{
    writeOutput("<statements>");
    ZeroOrMoreRule::compile();
    writeOutput("</statements>");
}
#pragma endregion

#pragma region StatementRule
StatementRule::StatementRule() : AlternationRule(
    {
        new LetStatementRule,
        new IfStatementRule,
        new WhileStatementRule,
        new DoStatementRule,
        new ReturnStatementRule
    })
{
}
#pragma endregion

#pragma region LetStatementRule
LetStatementRule::LetStatementRule() : SequenceRule(
    {
        new KeywordRule("let"),
        new VarNameRule,
        new ZeroOrOneRule([]
        {
            return new SequenceRule(
            {
                new SymbolRule('['),
                new ExpressionRule,
                new SymbolRule(']'),
            });
        }),
        new SymbolRule('='),
        new ExpressionRule,
        new SymbolRule(';')
    })
{
}

void LetStatementRule::compile()
{
    writeOutput("<letStatement>");
    SequenceRule::compile();
    writeOutput("</letStatement>");
}
#pragma endregion

#pragma region IfStatementRule
IfStatementRule::IfStatementRule() : SequenceRule(
    {
        new KeywordRule("if"),
        new SymbolRule('('),
        new ExpressionRule,
        new SymbolRule(')'),
        new SymbolRule('{'),
        new StatementsRule,
        new SymbolRule('}'),
        new ZeroOrOneRule([]
        {
            return new SequenceRule(
            {
                new KeywordRule("else"),
                new SymbolRule('{'),
                new StatementsRule,
                new SymbolRule('}')
            });
        })
    })
{
}

void IfStatementRule::compile()
{
    writeOutput("<ifStatement>");
    SequenceRule::compile();
    writeOutput("</ifStatement>");
}
#pragma endregion

#pragma region WhileStatementRule
WhileStatementRule::WhileStatementRule() : SequenceRule(
    {
        new KeywordRule("while"),
        new SymbolRule('('),
        new ExpressionRule,
        new SymbolRule(')'),
        new SymbolRule('{'),
        new StatementsRule,
        new SymbolRule('}')
    })
{
}

void WhileStatementRule::compile()
{
    writeOutput("<whileStatement>");
    SequenceRule::compile();
    writeOutput("</whileStatement>");
}
#pragma endregion

#pragma region DoStatementRule
DoStatementRule::DoStatementRule() : SequenceRule(
    {
        new KeywordRule("do"),
        new SubroutineCallRule,
        new SymbolRule(';')
    })
{
}

void DoStatementRule::compile()
{
    writeOutput("<doStatement>");
    SequenceRule::compile();
    writeOutput("</doStatement>");
}
#pragma endregion

#pragma region ReturnStatementRule
ReturnStatementRule::ReturnStatementRule() : SequenceRule(
    {
        new KeywordRule("return"),
        new ZeroOrOneRule([]
        {
            return new ExpressionRule;
        }),
        new SymbolRule(';')
    })
{
}

void ReturnStatementRule::compile()
{
    writeOutput("<returnStatement>");
    SequenceRule::compile();
    writeOutput("</returnStatement>");
}
#pragma endregion
