#include "BaseRules.h"
#include "ExpressionRules.h"
#include "LexicalRules.h"
#include "ProgramStructureRules.h"

ExpressionRule::ExpressionRule() : ParentRule(
    {
        /*new SequenceRule({
            new TermRule(),
            new ZeroOrMoreRule(
            {
                new OpRule(),
                new TermRule()
            })
        })*/
    })
{
}

TermRule::TermRule() : ParentRule(
    {
        new AlternationRule({
            new IntegerConstantRule(),
            new StringConstantRule(),
            new KeywordConstantRule(),
            new VarNameRule(),
            new SequenceRule(
            {
                new VarNameRule(),
                new SymbolRule('['),
                new ExpressionRule(),
                new SymbolRule(']'),
            }),
            new SequenceRule(
            {
                new SymbolRule('('),
                new ExpressionRule(),
                new SymbolRule(')'),
            }),
            new SequenceRule(
            {
                new UnaryOpRule(),
                new TermRule(),
            }),
            new SubroutineCallRule()
        })
    })
{
}

SubroutineCallRule::SubroutineCallRule() : ParentRule(
    {
        new AlternationRule({
            new SequenceRule(
            {
                new SubroutineNameRule(),
                new SymbolRule('('),
                new ExpressionListRule(),
                new SymbolRule(')')
            }),
            new SequenceRule(
            {
                new AlternationRule(
                {
                    new ClassNameRule(),
                    new VarNameRule()
                }),
                new SymbolRule('.'),
                new SubroutineNameRule(),
                new SymbolRule('('),
                new ExpressionListRule(),
                new SymbolRule(')')
            })
        })
    })
{
}

ExpressionListRule::ExpressionListRule() : ParentRule(
    {
        new ZeroOrOneRule(
        {
            new ExpressionRule(),
            new ZeroOrMoreRule(
            {
                new SymbolRule(','),
                new ExpressionRule(),
            })
        })
    })
{
}

OpRule::OpRule() : ParentRule(
    {
        new AlternationRule(
        {
            new SymbolRule('+'),
            new SymbolRule('-'),
            new SymbolRule('*'),
            new SymbolRule('/'),
            new SymbolRule('&'),
            new SymbolRule('|'),
            new SymbolRule('<'),
            new SymbolRule('>'),
            new SymbolRule('=')
        })
    })
{
}

UnaryOpRule::UnaryOpRule() : ParentRule(
    {
        new AlternationRule(
        {
            new SymbolRule('-'),
            new SymbolRule('~')
        })
    })
{
}

KeywordConstantRule::KeywordConstantRule() : ParentRule(
    {
        new AlternationRule(
        {
            new KeywordRule("true"),
            new KeywordRule("false"),
            new KeywordRule("null"),
            new KeywordRule("this")
        })
    })
{
}
