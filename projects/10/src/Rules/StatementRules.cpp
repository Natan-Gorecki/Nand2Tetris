#include "ExpressionRules.h"
#include "LexicalRules.h"
#include "ProgramStructureRules.h"
#include "StatementRules.h"

StatementsRule::StatementsRule() : ParentRule(
    {
        new ZeroOrMoreRule(
        {
            new StatementRule()
        })
    })
{
}

StatementRule::StatementRule() : ParentRule(
    {
        new AlternationRule(
        {
            new LetStatementRule(),
            new IfStatementRule(),
            new WhileStatementRule(),
            new DoStatementRule(),
            new ReturnStatementRule()
        })
    })
{
}

LetStatementRule::LetStatementRule() : ParentRule(
    {
        new SequenceRule(
        {
            new KeywordRule("let"),
            new VarNameRule(),
            new ZeroOrOneRule(
            {
                new SymbolRule('['),
                new ExpressionRule(),
                new SymbolRule(']'),
            }),
            new SymbolRule('='),
            new ExpressionRule(),
            new SymbolRule(';')
        })
    })
{
}

IfStatementRule::IfStatementRule() : ParentRule(
    {
        new SequenceRule(
        {
            new KeywordRule("if"),
            new SymbolRule('('),
            new ExpressionRule(),
            new SymbolRule(')'),
            new SymbolRule('{'),
            new StatementsRule(),
            new SymbolRule('}'),
            new ZeroOrOneRule(
            {
                new KeywordRule("else"),
                new SymbolRule('{'),
                new StatementsRule(),
                new SymbolRule('}')
            })
        })
    })
{
}

WhileStatementRule::WhileStatementRule() : ParentRule(
    {
        new SequenceRule(
        {
            new KeywordRule("while"),
            new SymbolRule('('),
            new ExpressionRule(),
            new SymbolRule(')'),
            new SymbolRule('{'),
            new StatementsRule(),
            new SymbolRule('}')
        })
    })
{
}

DoStatementRule::DoStatementRule() : ParentRule(
    {
        new SequenceRule(
        {
            new KeywordRule("do"),
            new SubroutineCallRule(),
            new SymbolRule(';')
        })
    })
{
}

ReturnStatementRule::ReturnStatementRule() : ParentRule(
    {
        new SequenceRule(
        {
            new KeywordRule("return"),
            new ZeroOrOneRule(
            {
                new ExpressionRule()
            }),
            new SymbolRule(';')
        })
    })
{
}
