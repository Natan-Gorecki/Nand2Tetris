#include <functional>
#include "ExpressionRules.h"
#include "ProgramStructureRules.h"
#include "StatementRules.h"
#include "../CompilationEngine.h"

using namespace std;

#pragma region ClassRule
ClassRule::ClassRule() : SequenceRule(
    {
        make_shared<KeywordRule>("class"),
        make_shared<ClassNameRule>(),
        make_shared<SymbolRule>('{'),
        make_shared<ZeroOrMoreRule>([]
        {
            return make_shared<ClassVarDecRule>();
        }),
        make_shared<ZeroOrMoreRule>([]
        {
            return make_shared<SubroutineDecRule>();
        }),
        make_shared<SymbolRule>('}')
    })
{
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
        make_shared<AlternationRule>(RuleVector
        {
            make_shared<KeywordRule>("static"),
            make_shared<KeywordRule>("field")
        }),
        make_shared<TypeRule>(),
        make_shared<VarNameRule>(),
        make_shared<ZeroOrMoreRule>([]
        {
            return make_shared<SequenceRule>(RuleVector
            {
                make_shared<SymbolRule>(','),
                make_shared<VarNameRule>()
            });
        }),
        make_shared<SymbolRule>(';')
    })
{
}

void ClassVarDecRule::compile()
{
    writeOutput("<classVarDec>");
    SequenceRule::compile();
    writeOutput("</classVarDec>");
}
#pragma endregion

#pragma region TypeRule
TypeRule::TypeRule() : AlternationRule(
    {
        make_shared<KeywordRule>("int"),
        make_shared<KeywordRule>("char"),
        make_shared<KeywordRule>("boolean"),
        make_shared<ClassNameRule>()
    })
{
}
#pragma endregion

#pragma region SubroutineDecRule
SubroutineDecRule::SubroutineDecRule() : SequenceRule(
    {
        make_shared<AlternationRule>(RuleVector
        {
            make_shared<KeywordRule>("constructor"),
            make_shared<KeywordRule>("function"),
            make_shared<KeywordRule>("method")
        }),
        make_shared<AlternationRule>(RuleVector
        {
            make_shared<KeywordRule>("void"),
            make_shared<TypeRule>()
        }),
        make_shared<SubroutineNameRule>(),
        make_shared<SymbolRule>('('),
        make_shared<ParameterListRule>(),
        make_shared<SymbolRule>(')'),
        make_shared<SubroutineBodyRule>()
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
        return make_shared<SequenceRule>(RuleVector
        {
            make_shared<TypeRule>(),
            make_shared<VarNameRule>(),
            make_shared<ZeroOrMoreRule>([]
            {
                return make_shared<SequenceRule>(RuleVector
                {
                    make_shared<SymbolRule>(','),
                    make_shared<TypeRule>(),
                    make_shared<VarNameRule>()
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
        make_shared<SymbolRule>('{'),
        make_shared<ZeroOrMoreRule>([]
        {
            return make_shared<VarDecRule>();
        }),
        make_shared<StatementsRule>(),
        make_shared<SymbolRule>('}')
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
        make_shared<KeywordRule>("var"),
        make_shared<TypeRule>(),
        make_shared<VarNameRule>(),
        make_shared<ZeroOrMoreRule>([]
        {
            return make_shared<SequenceRule>(RuleVector
            {
                make_shared<SymbolRule>(','),
                make_shared<VarNameRule>()
            });
        }),
        make_shared<SymbolRule>(';')
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
