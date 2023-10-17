#include "ProgramStructureRules.h"
#include "ExpressionRules.h"
#include "../CompilationEngine.h"

#pragma region ClassRule
ClassRule::ClassRule() : SequenceRule(
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
{
}

bool ClassRule::doCompile(JackTokenizer* pTokenizer)
{
    CompilationEngine::writeOutput("<class>\n");
    bool compileResult = SequenceRule::doCompile(pTokenizer);
    if (compileResult)
    {
        CompilationEngine::writeOutput("</class>\n");
    }

    return compileResult;
}
#pragma endregion

#pragma region ClassVarDecRule
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

bool ClassVarDecRule::doCompile(JackTokenizer* pTokenizer)
{
    CompilationEngine::writeOutput("<classVarDecRule>\n");
    CompilationEngine::writeOutput("</classVarDecRule>\n");
    return false;
}
#pragma endregion

#pragma region SubroutineDecRule
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

bool SubroutineDecRule::doCompile(JackTokenizer* pTokenizer)
{
    CompilationEngine::writeOutput("<subroutineDecRule>\n");
    CompilationEngine::writeOutput("</subroutineDecRule>\n");
    return false;
}
#pragma endregion

#pragma region others to implement


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
#pragma endregion