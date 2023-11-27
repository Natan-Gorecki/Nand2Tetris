#include <functional>
#include "ExpressionRules.h"
#include "ProgramStructureRules.h"
#include "RuleUtils.h"
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

SymbolTable& ClassRule::getSymbolTable()
{
    return mSymbolTable;
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

bool ClassVarDecRule::initialize(JackTokenizer* pTokenizer)
{
    if (!SequenceRule::initialize(pTokenizer))
    {
        return false;
    }

    auto classRule = RuleUtils::getParentRule<ClassRule>(this);
    auto& symbolTable = classRule->getSymbolTable();

    auto keywordRule = dynamic_cast<KeywordRule*>(getChildRule<AlternationRule>(0)->getPassedRule());
    auto symbolKind = keywordRule->toString() == "static" ? ESymbolKind::STATIC : ESymbolKind::FIELD;

    auto typeNameRule = dynamic_cast<LexicalRule*>(getChildRule<TypeRule>(1)->getPassedRule());
    auto type = typeNameRule->toString();

    auto name = getChildRule<LexicalRule>(2)->toString();

    symbolTable.define(name, type, symbolKind);

    for (const auto& childRule : getChildRule<ZeroOrMoreRule>(3)->getChildRules())
    {
        auto sequenceRule = dynamic_cast<SequenceRule*>(childRule.get());
        name = sequenceRule->getChildRule<VarNameRule>(1)->toString();

        symbolTable.define(name, type, symbolKind);
    }

    return true;
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

SymbolTable& SubroutineDecRule::getSymbolTable()
{
    return mSymbolTable;
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
