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

void ClassRule::writeXmlSyntax(std::ofstream* stream)
{
    writeXmlSyntaxImpl(stream, "<class>");
    SequenceRule::writeXmlSyntax(stream);
    writeXmlSyntaxImpl(stream, "</class>");
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

    auto keywordRule = getChildRule<AlternationRule>(0)->getPassedRule()->cast<KeywordRule>();
    auto symbolKind = keywordRule->toString() == "static" ? ESymbolKind::STATIC : ESymbolKind::FIELD;
    auto type = getChildRule<TypeRule>(1)->getPassedRule()->cast<LexicalRule>()->toString();
    auto name = getChildRule<LexicalRule>(2)->toString();

    symbolTable.define(name, type, symbolKind);

    for (const auto& childRule : getChildRule<ZeroOrMoreRule>(3)->getChildRules())
    {
        name = childRule.get()->cast<SequenceRule>()->getChildRule<VarNameRule>(1)->toString();
        symbolTable.define(name, type, symbolKind);
    }

    return true;
}

void ClassVarDecRule::writeXmlSyntax(std::ofstream* stream)
{
    writeXmlSyntaxImpl(stream, "<classVarDec>");
    SequenceRule::writeXmlSyntax(stream);
    writeXmlSyntaxImpl(stream, "</classVarDec>");
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

bool SubroutineDecRule::initialize(JackTokenizer* pTokenizer)
{
    if (!SequenceRule::initialize(pTokenizer))
    {
        return false;
    }

    auto subroutineType = getChildRule<AlternationRule>(0)->getPassedRule()->cast<KeywordRule>()->toString();
    if (subroutineType == "method")
    {
        auto classRule = RuleUtils::getParentRule<ClassRule>(this);
        auto className = classRule->getChildRule<ClassNameRule>(1)->toString();
        mSymbolTable.define("this", className, ESymbolKind::ARG, 0);
    }

    return true;
}

void SubroutineDecRule::compile(VMWriter* vmWriter)
{
    auto subroutineName = getChildRule<SubroutineNameRule>(2)->toString();
    auto classRule = RuleUtils::getParentRule<ClassRule>(this);
    auto className = classRule->getChildRule<ClassNameRule>(1)->toString();
    auto variablesCount = mSymbolTable.varCount(ESymbolKind::VAR);

    vmWriter->writeFunction(className + "." + subroutineName, variablesCount);

    auto subroutineType = getChildRule<AlternationRule>(0)->getPassedRule()->cast<KeywordRule>()->toString();
    if (subroutineType == "method")
    {
        vmWriter->writePush(ESegment::ARGUMENT, 0);
        vmWriter->writePop(ESegment::POINTER, 0);
    }

    SequenceRule::compile(vmWriter);

    auto returnType = getChildRule<AlternationRule>(1)->getPassedRule()->cast<LexicalRule>()->toString();
    if (returnType == "void")
    {
        vmWriter->writePush(ESegment::CONSTANT, 0);
    }

    vmWriter->writeReturn();
}

void SubroutineDecRule::writeXmlSyntax(std::ofstream* stream)
{
    writeXmlSyntaxImpl(stream, "<subroutineDec>");
    SequenceRule::writeXmlSyntax(stream);
    writeXmlSyntaxImpl(stream, "</subroutineDec>");
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

bool ParameterListRule::initialize(JackTokenizer* pTokenizer)
{
    ZeroOrOneRule::initialize(pTokenizer);

    if (getChildRules().size() == 0)
    {
        return true;
    }

    auto sequenceRule = getChildRule<SequenceRule>(0);
    auto& symbolTable = RuleUtils::getParentRule<SubroutineDecRule>(this)->getSymbolTable();
    auto type = sequenceRule->getChildRule<TypeRule>(0)->getPassedRule()->cast<LexicalRule>()->toString();
    auto name = sequenceRule->getChildRule<LexicalRule>(1)->toString();

    symbolTable.define(name, type, ESymbolKind::ARG);

    for (const auto& childRule : sequenceRule->getChildRule<ZeroOrMoreRule>(2)->getChildRules())
    {
        auto sequenceRule = childRule.get()->cast<SequenceRule>();
        type = sequenceRule->getChildRule<TypeRule>(1)->getPassedRule()->cast<LexicalRule>()->toString();
        name = sequenceRule->getChildRule<VarNameRule>(2)->toString();

        symbolTable.define(name, type, ESymbolKind::ARG);
    }

    return true;
}

void ParameterListRule::writeXmlSyntax(std::ofstream* stream)
{
    writeXmlSyntaxImpl(stream, "<parameterList>");
    ZeroOrOneRule::writeXmlSyntax(stream);
    writeXmlSyntaxImpl(stream, "</parameterList>");
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

bool SubroutineBodyRule::initialize(JackTokenizer* pTokenizer)
{
    if (!SequenceRule::initialize(pTokenizer))
    {
        return false;
    }

    auto& symbolTable = RuleUtils::getParentRule<SubroutineDecRule>(this)->getSymbolTable();
    for (const auto& childRule : getChildRule<ZeroOrMoreRule>(1)->getChildRules())
    {
        auto varDecRule = childRule->cast<VarDecRule>();

        auto type = varDecRule->getChildRule<TypeRule>(1)->getPassedRule()->cast<LexicalRule>()->toString();
        auto name = varDecRule->getChildRule<LexicalRule>(2)->toString();

        symbolTable.define(name, type, ESymbolKind::VAR);

        for (const auto& varDecChildRule : varDecRule->getChildRule<ZeroOrMoreRule>(3)->getChildRules())
        {
            name = varDecChildRule.get()->cast<SequenceRule>()->getChildRule<VarNameRule>(1)->toString();
            symbolTable.define(name, type, ESymbolKind::VAR);
        }
    }

    return true;
}

void SubroutineBodyRule::writeXmlSyntax(std::ofstream* stream)
{
    writeXmlSyntaxImpl(stream, "<subroutineBody>");
    SequenceRule::writeXmlSyntax(stream);
    writeXmlSyntaxImpl(stream, "</subroutineBody>");
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

void VarDecRule::writeXmlSyntax(std::ofstream* stream)
{
    writeXmlSyntaxImpl(stream, "<varDec>");
    SequenceRule::writeXmlSyntax(stream);
    writeXmlSyntaxImpl(stream, "</varDec>");
}
#pragma endregion
