#include "ProgramStructureRules.h"
#include "ExpressionRules.h"

ClassRule::ClassRule() : ParentRule(
    {
        new SequenceRule(
        {
            new KeywordRule("class"),
            new ClassNameRule(),
            new SymbolRule('{'),
            new ZeroOrMoreRule(
            {
                new ClassVarDecRule()
            }),
            new ZeroOrMoreRule(
            {
                new SubroutineDecRule()
            }),
            new SymbolRule('}')
        })
    })
{
}

ClassVarDecRule::ClassVarDecRule() : ParentRule(
    {
        new SequenceRule(
        {
            new AlternationRule(
            {
                new KeywordRule("static"),
                new KeywordRule("field")
            }),
            new TypeRule(),
            new VarNameRule(),
            new ZeroOrMoreRule(
            {
                new SymbolRule(','),
                new VarNameRule()
            }),
            new SymbolRule(';')
        })
    })
{
}

TypeRule::TypeRule() : ParentRule(
    {
        new AlternationRule(
        {
            new KeywordRule("int"),
            new KeywordRule("char"),
            new KeywordRule("boolean"),
            new ClassNameRule()
        })
    })
{
}

SubroutineDecRule::SubroutineDecRule() : ParentRule(
    {
        new SequenceRule(
        {
            new AlternationRule(
            {
                new KeywordRule("constructor"),
                new KeywordRule("function"),
                new KeywordRule("method")
            }),
            new AlternationRule(
            {
                new KeywordRule("void"),
                new TypeRule()
            }),
            new SubroutineNameRule(),
            new SymbolRule('('),
            new ParameterListRule(),
            new SymbolRule(')'),
            new SubroutineBodyRule()
        })
    })
{
}

ParameterListRule::ParameterListRule() : ParentRule(
    {
        new ZeroOrOneRule(
        {
            new TypeRule(),
            new VarNameRule(),
            new ZeroOrMoreRule(
            {
                new SymbolRule(','),
                new TypeRule(),
                new VarNameRule()
            })
        })
    })
{
}

SubroutineBodyRule::SubroutineBodyRule() : ParentRule(
    {
        new SequenceRule(
        {
            new AlternationRule(
            {
                new KeywordRule("constructor"),
                new KeywordRule("function"),
                new KeywordRule("method")
            }),
            new AlternationRule(
            {
                new KeywordRule("void"),
                new TypeRule()
            }),
            new SubroutineNameRule(),
            new SymbolRule('('),
            new ParameterListRule(),
            new SymbolRule(')'),
            new SubroutineBodyRule()
        })
    })
{
}

VarDecRule::VarDecRule() : ParentRule(
    {
        new SequenceRule(
        {
            new KeywordRule("var"),
            new TypeRule(),
            new VarNameRule(),
            new ZeroOrMoreRule(
            {
                new SymbolRule(','),
                new VarNameRule()
            }),
            new SymbolRule(';'),
        })
    })
{
}
