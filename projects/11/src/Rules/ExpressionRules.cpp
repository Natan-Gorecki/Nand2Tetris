#include <functional>
#include "BaseRules.h"
#include "ExpressionRules.h"
#include "LexicalRules.h"
#include "ProgramStructureRules.h"

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
    auto term1 = getChild(0)->cast<TermRule>();
    term1->compile(vmWriter);

    if (getChild(1)->getChildRules().size() > 0)
    {
        auto op = getChild(1)->getChild(0)->getChild(0)->cast<OpRule>();
        auto term2 = getChild(1)->getChild(0)->getChild(1)->cast<TermRule>();
        term2->compile(vmWriter);
        op->compile(vmWriter);
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
    auto sequenceRule = getChild(0)->cast<SequenceRule>();
    if (auto symbolRule = sequenceRule->getChild(0)->cast<SymbolRule>())
    {
        auto expressionRule = sequenceRule->getChild(1)->cast<ExpressionRule>();
        expressionRule->compile(vmWriter);
        return;
    }
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
    bool isMethod = getPassedRule()->getChild(0)->cast<SubroutineNameRule>() != nullptr;

    if (!isMethod)
    {
        auto className = getPassedRule()->getChild(0)->cast<AlternationRule>()->getPassedRule()->cast<ClassNameRule>()->toString();
        auto subroutineName = getPassedRule()->getChild(2)->cast<SubroutineNameRule>()->toString();
        auto expressionCount = getPassedRule()->getChild(4)->cast<ExpressionListRule>()->getExpressionCount();
        AlternationRule::compile(vmWriter);

        vmWriter->writeCall(className + "." + subroutineName, expressionCount);
    }
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

void ExpressionListRule::compile(VMWriter* vmWriter)
{
    SequenceRule::compile(vmWriter);
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
    AlternationRule::compile(vmWriter);

    auto symbol = getPassedRule()->cast<SymbolRule>()->getValue();
    switch (symbol)
    {
    case '+':
        vmWriter->writeArithmetic(EArithmetic::ADD);
        break;
    case '*':
        vmWriter->writeCall("Math.multiply", 2);
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
#pragma endregion
