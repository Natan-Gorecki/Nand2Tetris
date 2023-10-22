#include "ExpressionRules.h"
#include "ProgramStructureRules.h"
#include "StatementRules.h"
#include "../CompilationEngine.h"

#pragma region ClassRule
ClassRule::ClassRule() : SequenceRule(
    {
        new KeywordRule("class"),
        new ClassNameRule,
        new SymbolRule('{'),
        new ZeroOrMoreRule([]
        {
            return new ClassVarDecRule;
        }),
        new ZeroOrMoreRule([]
        {
            return new SubroutineDecRule;
        }),
        new SymbolRule('}')
    })
{
    CreateRuleFunc createRuleFunc = []() { return new ClassVarDecRule; };
}

void ClassRule::compile()
{
    writeOutput("<class>");
    SequenceRule::compile();
    writeOutput("</class>");
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
        new TypeRule,
        new VarNameRule,
        new ZeroOrMoreRule([]
        {
            return new SequenceRule(
            {
                new SymbolRule(','),
                new VarNameRule
            });
        }),
        new SymbolRule(';')
    })
{
}

void ClassVarDecRule::compile()
{
    writeOutput("<classVarDecRule>");
    SequenceRule::compile();
    writeOutput("</classVarDecRule>");
}
#pragma endregion

#pragma region TypeRule
TypeRule::TypeRule() : AlternationRule(
    {
        new KeywordRule("int"),
        new KeywordRule("char"),
        new KeywordRule("boolean"),
        new ClassNameRule
    })
{
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
            new TypeRule
        }),
        new SubroutineNameRule,
        new SymbolRule('('),
        new ParameterListRule,
        new SymbolRule(')'),
        new SubroutineBodyRule
    })
{
}

void SubroutineDecRule::compile()
{
    writeOutput("<subroutineDec>");
    SequenceRule::compile();
    writeOutput("</subroutineDec>");
}
#pragma endregion

#pragma region ParameterListRule
ParameterListRule::ParameterListRule() : ZeroOrOneRule([]
    {
        return new SequenceRule(
        {
            new TypeRule,
            new VarNameRule,
            new ZeroOrMoreRule([]
            {
                return new SequenceRule(
                {
                    new SymbolRule(','),
                    new TypeRule,
                    new VarNameRule
                });
            })
        });
    })
{
}

void ParameterListRule::compile()
{
    writeOutput("<parameterList>");
    ZeroOrOneRule::compile();
    writeOutput("</parameterList>");
}
#pragma endregion

#pragma region SubroutineBodyRule
SubroutineBodyRule::SubroutineBodyRule() : SequenceRule(
    {
        new SymbolRule('{'),
        new ZeroOrMoreRule([]
        {
            return new VarDecRule;
        }),
        new StatementsRule,
        new SymbolRule('}')
    })
{
}

void SubroutineBodyRule::compile()
{
    writeOutput("<subroutineBody>");
    SequenceRule::compile();
    writeOutput("</subroutineBody>");
}
#pragma endregion

#pragma region VarDecRule
VarDecRule::VarDecRule() : SequenceRule(
    {
        new KeywordRule("var"),
        new TypeRule,
        new VarNameRule,
        new ZeroOrMoreRule([]
        {
            return new SequenceRule(
            {
                new SymbolRule(','),
                new VarNameRule
            });
        }),
        new SymbolRule(';')
    })
{
}

void VarDecRule::compile()
{
    writeOutput("<varDec>");
    SequenceRule::compile();
    writeOutput("</varDec>");
}
#pragma endregion
