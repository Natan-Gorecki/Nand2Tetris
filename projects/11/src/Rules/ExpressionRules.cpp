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
    writeOutput("<expression>");
    auto term1 = getChildRule<TermRule>(0);
    term1->compile(vmWriter);

    if (getChildRule<ZeroOrMoreRule>(1)->getChildRules().size() > 0)
    {
        auto op = getChildRule<ZeroOrMoreRule>(1)->getChildRule<SequenceRule>(0)->getChildRule<OpRule>(0);
        auto term2 = getChildRule<ZeroOrMoreRule>(1)->getChildRule<SequenceRule>(0)->getChildRule<TermRule>(1);
        term2->compile(vmWriter);
        op->compile(vmWriter);
    }
    writeOutput("</expression>");
}
#pragma endregion

#pragma region TermRule
TermRule::TermRule() : ParentRule({ })
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
        pRule->setParentRule(this);

        const Rule& rule = *pRule;
        int ruleLevel = typeid(rule) != typeid(SequenceRule) ? getRuleLevel() + 1 : getRuleLevel();
        pRule->setRuleLevel(ruleLevel);

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
    if (auto integerRule = getChildRule<IntegerConstantRule>(0))
    {
        vmWriter->writePush(ESegment::CONSTANT, integerRule->getValue());
    }
    writeOutput("<term>");
    ParentRule::compile(vmWriter);
    writeOutput("</term>");
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
    bool isMethod = getPassedRule()->cast<SequenceRule>()->getChildRule<SubroutineNameRule>(0) != nullptr;

    if (!isMethod)
    {
        auto className = getPassedRule()->cast<SequenceRule>()->getChildRule<AlternationRule>(0)->getPassedRule()->cast<ClassNameRule>()->toString();
        auto subroutineName = getPassedRule()->cast<SequenceRule>()->getChildRule<SubroutineNameRule>(2)->toString();
        auto expressionCount = getPassedRule()->cast<SequenceRule>()->getChildRule<ExpressionListRule>(4)->getExpressionCount();
        AlternationRule::compile(vmWriter);

        vmWriter->writeCall(className + "." + subroutineName, expressionCount);
    }
}

void SubroutineCallRule::setRuleLevel(int ruleLevel)
{
    AlternationRule::setRuleLevel(ruleLevel - 1);
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
    writeOutput("<expressionList>");
    SequenceRule::compile(vmWriter);
    writeOutput("</expressionList>");
}

int ExpressionListRule::getExpressionCount()
{
    if (getChildRules().empty())
    {
        return 0;
    }

    auto zeroOrMoreRule = getChildRule<ZeroOrOneRule>(0)->getChildRule<SequenceRule>(0)->getChildRule<ZeroOrMoreRule>(1);
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
