#include <typeinfo>
#include "BaseRules.h"
#include "../CompilationEngine.h"
#include "../JackAnalyzerError.h"

using namespace std;

#pragma region Rule
bool Rule::initialize(JackTokenizer* pTokenizer)
{
    // default virtual empty method
    return true;
}

void Rule::compile()
{
    // default virtual empty method
}

int Rule::getRuleLevel() const
{
    return mRuleLevel;
}

void Rule::setRuleLevel(int ruleLevel)
{
    mRuleLevel = ruleLevel;
}

void Rule::writeOutput(string const& text) const
{
    auto tabs = string(mRuleLevel * 2, ' ');
    auto formattedOutput = tabs + text + "\n";
    CompilationEngine::writeOutput(formattedOutput);
}

void Rule::writeToken(string const& text) const
{
    auto formattedOutput = text + "\n";
    CompilationEngine::writeToken(formattedOutput);
}
#pragma endregion

#pragma region ParentRule
ParentRule::ParentRule(RuleVector const& rules)
{
    for (const auto& pRule : rules)
    {
        mChildRules.push_back(pRule);
    }
}

void ParentRule::compile()
{
    for (const auto& pRule : mChildRules)
    {
        pRule->compile();
    }
}

RuleVector& ParentRule::getChildRules()
{
    return mChildRules;
}
#pragma endregion

#pragma region SequenceRule
SequenceRule::SequenceRule(RuleVector const& rules)
    : ParentRule(rules)
{
}

bool SequenceRule::initialize(JackTokenizer* pTokenizer)
{
    auto initialPosition = pTokenizer->getPosition();
    for (const auto& pRule : getChildRules())
    {
        pRule->setRuleLevel(getRuleLevel() + 1);
        auto result = pRule->initialize(pTokenizer);
        
        if (!result)
        {
            pTokenizer->setPosition(initialPosition);
            return false;
        }
    }

    return true;
}
#pragma endregion

#pragma region AlternationRule
AlternationRule::AlternationRule(RuleVector const& rules)
    : ParentRule(rules)
{
}

bool AlternationRule::initialize(JackTokenizer* pTokenizer)
{
    for (const auto& pRule : getChildRules())
    {
        auto result = pRule->initialize(pTokenizer);

        if (result)
        {
            mCompileRule = pRule;
            return true;
        }
    }

    return false;
}

void AlternationRule::compile()
{
    if (!mCompileRule)
    {
        throw JackAnalyzerError("Failed to compile AlternationRule.");
    }

    mCompileRule->compile();
}

void AlternationRule::setRuleLevel(int ruleLevel)
{
    ParentRule::setRuleLevel(ruleLevel);
    for (const auto& pRule : getChildRules())
    {
        pRule->setRuleLevel(ruleLevel);
    }
}
#pragma endregion

#pragma region ZeroOrMoreRule
ZeroOrMoreRule::ZeroOrMoreRule(CreateRuleFunc const& createRuleFunc)
    : ParentRule({ }),
    onCreateRule(createRuleFunc)
{
}

bool ZeroOrMoreRule::initialize(JackTokenizer* pTokenizer)
{
    bool result = true;

    while (result)
    {
        auto pRule = onCreateRule();

        const Rule& rule = *pRule;
        int ruleLevel = typeid(rule) != typeid(SequenceRule) ? getRuleLevel() : getRuleLevel() - 1;
        pRule->setRuleLevel(ruleLevel);

        if (!pRule->initialize(pTokenizer))
        {
            break;
        }

        getChildRules().push_back(pRule);
    }

    return true;
}
#pragma endregion

#pragma region ZeroOrOneRule
ZeroOrOneRule::ZeroOrOneRule(CreateRuleFunc const& createRuleFunc)
    : ParentRule({ }),
    onCreateRule(createRuleFunc)
{
}

bool ZeroOrOneRule::initialize(JackTokenizer* pTokenizer)
{
    auto pRule = onCreateRule();

    const Rule& rule = *pRule;
    int ruleLevel = typeid(rule) != typeid(SequenceRule) ? getRuleLevel() : getRuleLevel() - 1;
    pRule->setRuleLevel(ruleLevel);

    if (pRule->initialize(pTokenizer))
    {
        getChildRules().push_back(pRule);
    }

    return true;
}
#pragma endregion
