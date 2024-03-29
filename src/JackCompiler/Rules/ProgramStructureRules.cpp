#include <functional>
#include "ExpressionRules.h"
#include "ProgramStructureRules.h"
#include "StatementRules.h"
#include "../CompilationEngine.h"
#include "../JackCompilerError.h"

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

void ClassRule::writeXmlSyntax(std::ofstream* stream, int tabs)
{
    XML_SYNTAX("<class>");
    SequenceRule::writeXmlSyntax(stream, tabs);
    XML_SYNTAX("</class>");
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

    auto& classTable = getParentRecursive<ClassRule>()->getSymbolTable();

    auto keywordRule = getChild(0)->cast<AlternationRule>()->getTrueRule()->cast<KeywordRule>();
    auto symbolKind = keywordRule->toString() == "static" ? ESymbolKind::STATIC : ESymbolKind::FIELD;
    auto type = getChild(1)->cast<TypeRule>()->getTrueRule()->cast<LexicalRule>()->toString();

    auto name = getChild(2)->cast<VarNameRule>()->toString();
    classTable.define(name, type, symbolKind);

    for (const auto& childRule : getChild(3)->getChildRules())
    {
        name = childRule.get()->getChild(1)->cast<VarNameRule>()->toString();
        classTable.define(name, type, symbolKind);
    }

    return true;
}

void ClassVarDecRule::writeXmlSyntax(std::ofstream* stream, int tabs)
{
    XML_SYNTAX("<classVarDec>");
    SequenceRule::writeXmlSyntax(stream, tabs);
    XML_SYNTAX("</classVarDec>");
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

    if (auto subroutineType = getChild(0)->cast<AlternationRule>()->getTrueRule()->cast<KeywordRule>()->toString(); subroutineType == "method")
    {
        auto classRule = getParentRecursive<ClassRule>();
        auto className = classRule->getChild(1)->cast<ClassNameRule>()->toString();
        mSymbolTable.define("this", className, ESymbolKind::ARG, 0);
    }

    return true;
}

void SubroutineDecRule::compile(VMWriter* vmWriter)
{
    auto subroutineName = getChild(2)->cast<SubroutineNameRule>()->toString();
    auto className = getParentRecursive<ClassRule>()->getChild(1)->cast<ClassNameRule>()->toString();
    auto variablesCount = mSymbolTable.varCount(ESymbolKind::VAR);

    vmWriter->writeFunction(className + "." + subroutineName, variablesCount);

    auto subroutineType = getChild(0)->cast<AlternationRule>()->getTrueRule()->cast<KeywordRule>()->toString();
    if (subroutineType == "method")
    {
        vmWriter->writePush(ESegment::ARGUMENT, 0);
        vmWriter->writePop(ESegment::POINTER, 0);
    }
    if (subroutineType == "constructor")
    {
        auto fieldsCount = getParentRecursive<ClassRule>()->getSymbolTable().varCount(ESymbolKind::FIELD);
        vmWriter->writePush(ESegment::CONSTANT, fieldsCount);
        vmWriter->writeCall("Memory.alloc", 1);
        vmWriter->writePop(ESegment::POINTER, 0);
    }

    getChild(6)->compile(vmWriter);

    if (auto keywordRule = getChild(1)->cast<AlternationRule>()->getTrueRule()->cast<KeywordRule>())
    {
        vmWriter->writePush(ESegment::CONSTANT, 0);
        vmWriter->writeReturn();
    }
}

void SubroutineDecRule::writeXmlSyntax(std::ofstream* stream, int tabs)
{
    XML_SYNTAX("<subroutineDec>");
    SequenceRule::writeXmlSyntax(stream, tabs);
    XML_SYNTAX("</subroutineDec>");
}

SymbolTable& SubroutineDecRule::getSymbolTable()
{
    return mSymbolTable;
}

int SubroutineDecRule::getUniqueNumber(Rule* rule)
{
    if (auto whileRule = rule->cast<WhileStatementRule>())
    {
        return whileCounter++;
    }
    if (auto ifRule = rule->cast<IfStatementRule>())
    {
        return ifCounter++;
    }
    throw JackCompilerError("Unique number is unsupported for rule.");
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

    auto& subroutineTable = getParentRecursive<SubroutineDecRule>()->getSymbolTable();
    auto type = getChild(0)->getChild(0)->cast<TypeRule>()->getTrueRule()->cast<LexicalRule>()->toString();
    auto name = getChild(0)->getChild(1)->cast<VarNameRule>()->toString();
    subroutineTable.define(name, type, ESymbolKind::ARG);

    for (const auto& childRule : getChild(0)->getChild(2)->getChildRules())
    {
        type = childRule->getChild(1)->cast<TypeRule>()->getTrueRule()->cast<LexicalRule>()->toString();
        name = childRule->getChild(2)->cast<VarNameRule>()->toString();
        subroutineTable.define(name, type, ESymbolKind::ARG);
    }

    return true;
}

void ParameterListRule::writeXmlSyntax(std::ofstream* stream, int tabs)
{
    XML_SYNTAX("<parameterList>");
    ZeroOrOneRule::writeXmlSyntax(stream, tabs);
    XML_SYNTAX("</parameterList>");
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

    auto& subroutineTable = getParentRecursive<SubroutineDecRule>()->getSymbolTable();
    for (const auto& varDecRule : getChild(1)->getChildRules())
    {
        auto type = varDecRule->getChild(1)->cast<TypeRule>()->getTrueRule()->cast<LexicalRule>()->toString();

        auto name = varDecRule->getChild(2)->cast<LexicalRule>()->toString();
        subroutineTable.define(name, type, ESymbolKind::VAR);

        for (const auto& sequenceRule : varDecRule->getChild(3)->getChildRules())
        {
            name = sequenceRule.get()->getChild(1)->cast<VarNameRule>()->toString();
            subroutineTable.define(name, type, ESymbolKind::VAR);
        }
    }

    return true;
}

void SubroutineBodyRule::writeXmlSyntax(std::ofstream* stream, int tabs)
{
    XML_SYNTAX("<subroutineBody>");
    SequenceRule::writeXmlSyntax(stream, tabs);
    XML_SYNTAX("</subroutineBody>");
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

void VarDecRule::writeXmlSyntax(std::ofstream* stream, int tabs)
{
    XML_SYNTAX("<varDec>");
    SequenceRule::writeXmlSyntax(stream, tabs);
    XML_SYNTAX("</varDec>");
}
#pragma endregion
