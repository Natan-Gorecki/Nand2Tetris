#include "ExpressionRules.h"
#include "ProgramStructureRules.h"
#include "StatementRules.h"
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
ClassVarDecRule::ClassVarDecRule() : SequenceRule(
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
{
}

bool ClassVarDecRule::doCompile(JackTokenizer* pTokenizer)
{
    CompilationEngine::writeOutput("<classVarDecRule>\n");
    bool compileResult = SequenceRule::doCompile(pTokenizer);
    if (compileResult)
    {
        CompilationEngine::writeOutput("</classVarDecRule>\n");
    }
    return compileResult;
}
#pragma endregion

#pragma region TypeRule
TypeRule::TypeRule() : AlternationRule(
    {
        new KeywordRule("int"),
        new KeywordRule("char"),
        new KeywordRule("boolean"),
        new ClassNameRule()
    })
{
}

bool TypeRule::doCompile(JackTokenizer* pTokenizer)
{
    return AlternationRule::doCompile(pTokenizer);
}
#pragma endregion

#pragma region SubroutineDecRule
SubroutineDecRule::SubroutineDecRule() : SequenceRule(
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
{
}

bool SubroutineDecRule::doCompile(JackTokenizer* pTokenizer)
{
    CompilationEngine::writeOutput("<subroutineDecRule>\n");
    bool compileResult = SequenceRule::doCompile(pTokenizer);
    if (compileResult)
    {
        CompilationEngine::writeOutput("</subroutineDecRule>\n");
    }
    return compileResult;
}
#pragma endregion

#pragma region ParameterListRule
ParameterListRule::ParameterListRule() : ZeroOrOneRule(
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
{
}

bool ParameterListRule::doCompile(JackTokenizer* pTokenizer)
{
    CompilationEngine::writeOutput("<parameterList>\n");
    auto compileResult = ZeroOrOneRule::doCompile(pTokenizer);
    CompilationEngine::writeOutput("</parameterList>\n");
    return compileResult;
}
#pragma endregion

#pragma region SubroutineBodyRule
SubroutineBodyRule::SubroutineBodyRule() : SequenceRule(
    {
        new SymbolRule('{'),
        new ZeroOrMoreRule(
        {
            new VarDecRule()
        }),
        new StatementsRule(),
        new SymbolRule('}')
    })
{
}

bool SubroutineBodyRule::doCompile(JackTokenizer* pTokenizer)
{
    CompilationEngine::writeOutput("<subroutineBody>\n");
    bool compileResult = SequenceRule::doCompile(pTokenizer);
    if (compileResult)
    {
        CompilationEngine::writeOutput("</subroutineBody>\n");
    }
    return compileResult;
}
#pragma endregion

#pragma region VarDecRule
VarDecRule::VarDecRule() : SequenceRule(
    {
        new KeywordRule("var"),
        new TypeRule(),
        new VarNameRule(),
        new ZeroOrMoreRule(
        {
            new SymbolRule(','),
            new VarNameRule()
        }),
        new SymbolRule(';')
    })
{
}

bool VarDecRule::doCompile(JackTokenizer* pTokenizer)
{
    CompilationEngine::writeOutput("<varDec>\n");
    bool compileResult = SequenceRule::doCompile(pTokenizer);
    if (compileResult)
    {
        CompilationEngine::writeOutput("</varDec>\n");
    }
    return compileResult;
}
#pragma endregion
