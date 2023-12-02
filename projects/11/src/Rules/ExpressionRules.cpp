#include <functional>
#include "BaseRules.h"
#include "ExpressionRules.h"
#include "LexicalRules.h"
#include "ProgramStructureRules.h"
#include "RuleUtils.h"

using namespace std;

#pragma region ExpressionRule
ExpressionRule::ExpressionRule() : SequenceRule(
    {
        make_shared<TermRule>(),
        make_shared<ZeroOrMoreRule>([]
        {
            return make_shared<SequenceRule>(RuleVector
            {
                make_shared<OpRule>(),
                make_shared<TermRule>()
            });
        })
    })
{
}

void ExpressionRule::compile(VMWriter* vmWriter)
{
    getChild(0)->compile(vmWriter);

    for (const auto& rule : getChild(1)->getChildRules())
    {
        rule->getChild(1)->compile(vmWriter);
        rule->getChild(0)->compile(vmWriter);
    }
}

void ExpressionRule::writeXmlSyntax(ofstream* stream, int tabs)
{
    XML_SYNTAX("<expression>");
    SequenceRule::writeXmlSyntax(stream, tabs);
    XML_SYNTAX("</expression>");
}
#pragma endregion

#pragma region TermRule
TermRule::TermRule() : Rule({ })
{
    mCreateRuleFuncs =
    {
        [] { return make_shared<SubroutineCallRule>(); },
        [] { return make_shared<SequenceRule>(RuleVector
        {
            make_shared<UnaryOpRule>(),
            make_shared<TermRule>(),
        }); },
        [] { return make_shared<SequenceRule>(RuleVector
        {
            make_shared<SymbolRule>('('),
            make_shared<ExpressionRule>(),
            make_shared<SymbolRule>(')'),
        }); },
        [] { return make_shared<SequenceRule>(RuleVector
        {
            make_shared<VarNameRule>(),
            make_shared<SymbolRule>('['),
            make_shared<ExpressionRule>(),
            make_shared<SymbolRule>(']'),
        }); },
        [] { return make_shared<IntegerConstantRule>(); },
        [] { return make_shared<StringConstantRule>(); },
        [] { return make_shared<KeywordConstantRule>(); },
        [] { return make_shared<VarNameRule>(); }
    };
}

bool TermRule::initialize(JackTokenizer* pTokenizer)
{
    for (const auto& onCreateRule : mCreateRuleFuncs)
    {
        auto pRule = onCreateRule();
        pRule->setParent(this);

        if (pRule->initialize(pTokenizer))
        {
            getChildRules().push_back(pRule);
            return true;
        }
    }

    return false;
}

void TermRule::compile(VMWriter* vmWriter)
{
    if (auto integerRule = getChild(0)->cast<IntegerConstantRule>())
    {
        vmWriter->writePush(ESegment::CONSTANT, integerRule->getValue());
        return;
    }
    if (auto stringRule = getChild(0)->cast<StringConstantRule>())
    {
        auto stringValue = stringRule->toString();
        vmWriter->writePush(ESegment::CONSTANT, stringValue.length());
        vmWriter->writeCall("String.new", 1);

        for (int i = 0; i < stringValue.length(); i++)
        {
            vmWriter->writePush(ESegment::CONSTANT, stringValue[i]);
            vmWriter->writeCall("String.appendChar", 2);
        }
        return;
    }
    if (auto keywordRule = getChild(0)->cast<KeywordConstantRule>())
    {
        keywordRule->compile(vmWriter);
        return;
    }
    if (auto varNameRule = getChild(0)->cast<VarNameRule>())
    {
        auto symbol = RuleUtils::findSymbol(this, varNameRule->toString());
        vmWriter->writePush(symbol.getSegment(), symbol.index);
        return;
    }
    if (auto subroutineRule = getChild(0)->cast<SubroutineCallRule>())
    {
        subroutineRule->compile(vmWriter);
        return;
    }

    auto sequenceRule = getChild(0)->cast<SequenceRule>();
    if (sequenceRule->getChild(0)->cast<UnaryOpRule>())
    {
        sequenceRule->getChild(1)->compile(vmWriter);
        sequenceRule->getChild(0)->compile(vmWriter);
        return;
    }
    if (sequenceRule->getChild(0)->cast<SymbolRule>())
    {
        sequenceRule->getChild(0)->compile(vmWriter);
        return;
    }

    // arrays
    auto symbol = RuleUtils::findSymbol(this, sequenceRule->getChild(0)->cast<VarNameRule>()->toString());
    vmWriter->writePush(symbol.getSegment(), symbol.index);
    sequenceRule->getChild(2)->compile(vmWriter);
    vmWriter->writeArithmetic(EArithmetic::ADD);
    vmWriter->writePop(ESegment::POINTER, 1);
    vmWriter->writePush(ESegment::THAT, 0);
}

void TermRule::writeXmlSyntax(ofstream* stream, int tabs)
{
    XML_SYNTAX("<term>");

    int ruleTabs = tabs + 1;
    if (getChildRules().size() > 0)
    {
        const Rule& rule = *getChild(0);
        if (typeid(rule) == typeid(SequenceRule))
        {
            ruleTabs = tabs;
        }
    }
    Rule::writeXmlSyntax(stream, ruleTabs);

    XML_SYNTAX("</term>");
}
#pragma endregion

#pragma region SubroutineCallRule
SubroutineCallRule::SubroutineCallRule() : AlternationRule(
    {
        make_shared<SequenceRule>(RuleVector
        {
            make_shared<SubroutineNameRule>(),
            make_shared<SymbolRule>('('),
            make_shared<ExpressionListRule>(),
            make_shared<SymbolRule>(')')
        }),
        make_shared<SequenceRule>(RuleVector
        {
            make_shared<AlternationRule>(RuleVector
            {
                make_shared<ClassNameRule>(),
                make_shared<VarNameRule>()
            }),
            make_shared<SymbolRule>('.'),
            make_shared<SubroutineNameRule>(),
            make_shared<SymbolRule>('('),
            make_shared<ExpressionListRule>(),
            make_shared<SymbolRule>(')')
        })
    })
{
}

void SubroutineCallRule::compile(VMWriter* vmWriter)
{
    string className, subroutineName;
    ExpressionListRule* expressionListRule = nullptr;

    // subroutineName(expressionList)
    if (auto subroutineNameRule = getTrueRule()->getChild(0)->cast<SubroutineNameRule>())
    {
        className = getParentRecursive<ClassRule>()->getChild(1)->cast<ClassNameRule>()->toString();
        subroutineName = subroutineNameRule->toString();
        expressionListRule = getTrueRule()->getChild(2)->cast<ExpressionListRule>();

        vmWriter->writePush(ESegment::POINTER, 0);
        expressionListRule->compile(vmWriter);
        
        vmWriter->writeCall(className + "." + subroutineName, expressionListRule->getExpressionCount() + 1);
        return;
    }

    // varName.subroutineName(expressionList)
    if (auto varNameRule = getTrueRule()->getChild(0)->cast<AlternationRule>()->getTrueRule()->cast<VarNameRule>())
    {
        auto symbol = RuleUtils::findSymbol(this, varNameRule->toString());
        subroutineName = getTrueRule()->getChild(2)->cast<SubroutineNameRule>()->toString();
        expressionListRule = getTrueRule()->getChild(4)->cast<ExpressionListRule>();

        vmWriter->writePush(symbol.getSegment(), symbol.index);
        expressionListRule->compile(vmWriter);

        vmWriter->writeCall(className + "." + subroutineName, expressionListRule->getExpressionCount() + 1);
        return;
    }

    // className.subroutineName(expressionList)
    className = getTrueRule()->getChild(0)->cast<AlternationRule>()->getTrueRule()->cast<ClassNameRule>()->toString();
    subroutineName = getTrueRule()->getChild(2)->cast<SubroutineNameRule>()->toString();
    expressionListRule = getTrueRule()->getChild(4)->cast<ExpressionListRule>();

    expressionListRule->compile(vmWriter);
    vmWriter->writeCall(className + "." + subroutineName, expressionListRule->getExpressionCount());
}

void SubroutineCallRule::writeXmlSyntax(std::ofstream* stream, int tabs)
{
    AlternationRule::writeXmlSyntax(stream, tabs - 1);
}
#pragma endregion

#pragma region ExpressionListRule
ExpressionListRule::ExpressionListRule() : SequenceRule(
    {
        make_shared<ZeroOrOneRule>([]
        {
            return make_shared<SequenceRule>(RuleVector
            {
                make_shared<ExpressionRule>(),
                make_shared<ZeroOrMoreRule>([]
                {
                    return make_shared<SequenceRule>(RuleVector
                    {
                        make_shared<SymbolRule>(','),
                        make_shared<ExpressionRule>(),
                    });
                })
            });
        })
    })
{
}

void ExpressionListRule::writeXmlSyntax(std::ofstream* stream, int tabs)
{
    XML_SYNTAX("<expressionList>");
    SequenceRule::writeXmlSyntax(stream, tabs);
    XML_SYNTAX("</expressionList>");
}

int ExpressionListRule::getExpressionCount()
{
    if (getChildRules().empty())
    {
        return 0;
    }

    auto zeroOrMoreRule = getChild(0)->getChild(0)->getChild(1)->cast<ZeroOrMoreRule>();
    return 1 + zeroOrMoreRule->getChildRules().size();
}
#pragma endregion

#pragma region OpRule
OpRule::OpRule() : AlternationRule(
    {
        make_shared<SymbolRule>('+'),
        make_shared<SymbolRule>('-'),
        make_shared<SymbolRule>('*'),
        make_shared<SymbolRule>('/'),
        make_shared<SymbolRule>('&'),
        make_shared<SymbolRule>('|'),
        make_shared<SymbolRule>('<'),
        make_shared<SymbolRule>('>'),
        make_shared<SymbolRule>('=')
    })
{
}

void OpRule::compile(VMWriter* vmWriter)
{
    auto symbol = getTrueRule()->cast<SymbolRule>()->getValue();
    switch (symbol)
    {
    case '+':
        vmWriter->writeArithmetic(EArithmetic::ADD);
        break;
    case '-':
        vmWriter->writeArithmetic(EArithmetic::SUB);
        break;
    case '*':
        vmWriter->writeCall("Math.multiply", 2);
        break;
    case '/':
        vmWriter->writeCall("Math.divide", 2);
        break;
    case '&':
        vmWriter->writeArithmetic(EArithmetic::AND);
        break;
    case '|':
        vmWriter->writeArithmetic(EArithmetic::OR);
        break;
    case '<':
        vmWriter->writeArithmetic(EArithmetic::LT);
        break;
    case '>':
        vmWriter->writeArithmetic(EArithmetic::GT);
        break;
    case '=':
        vmWriter->writeArithmetic(EArithmetic::EQ);
        break;
    }
}
#pragma endregion

#pragma region UnaryOpRule
UnaryOpRule::UnaryOpRule() : AlternationRule(
    {
        make_shared<SymbolRule>('-'),
        make_shared<SymbolRule>('~')
    })
{
}

void UnaryOpRule::compile(VMWriter* vmWriter)
{
    auto symbol = getTrueRule()->cast<SymbolRule>()->getValue();
    switch (symbol)
    {
    case '-':
        vmWriter->writeArithmetic(EArithmetic::NEG);
        break;
    case '~':
        vmWriter->writeArithmetic(EArithmetic::NOT);
        break;
    }
}
#pragma endregion

#pragma region KeywordConstantRule
KeywordConstantRule::KeywordConstantRule() : AlternationRule(
    {
        make_shared<KeywordRule>("true"),
        make_shared<KeywordRule>("false"),
        make_shared<KeywordRule>("null"),
        make_shared<KeywordRule>("this")
    })
{
}

void KeywordConstantRule::compile(VMWriter* vmWriter)
{
    auto constant = getTrueRule()->cast<KeywordRule>()->toString();

    if (constant == "true")
    {
        vmWriter->writePush(ESegment::CONSTANT, 1);
        vmWriter->writeArithmetic(EArithmetic::NEG);
        return;
    }

    if (constant == "this")
    {
        vmWriter->writePush(ESegment::POINTER, 0);
        return;
    }

    vmWriter->writePush(ESegment::CONSTANT, 0);
}
#pragma endregion
