#include <typeinfo>
#include "BaseRules.h"
#include "../CompilationEngine.h"
#include "../JackCompilerError.h"

using namespace std;

#pragma region Rule
bool Rule::initialize(JackTokenizer* pTokenizer)
{
    // default virtual empty method
    return true;
}

void Rule::compile(VMWriter* vmWriter)
{
    // default virtual empty method
}

Rule* Rule::getParentRule()
{
    return mParentRule;
}

void Rule::setParentRule(Rule* pRule)
{
    mParentRule = pRule;
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

bool ParentRule::initialize(JackTokenizer* pTokenizer)
{
    for (const auto& pRule : mChildRules)
    {
        pRule->setParentRule(this);
    }
    return true;
}

void ParentRule::compile(VMWriter* vmWriter)
{
    for (const auto& pRule : mChildRules)
    {
        pRule->compile(vmWriter);
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
    ParentRule::initialize(pTokenizer);

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
    ParentRule::initialize(pTokenizer);

    for (const auto& pRule : getChildRules())
    {
        auto result = pRule->initialize(pTokenizer);

        if (result)
        {
            mPassedRule = pRule;
            return true;
        }
    }

    return false;
}

void AlternationRule::compile(VMWriter* vmWriter)
{
    if (!mPassedRule)
    {
        throw JackCompilerError("Failed to compile AlternationRule.");
    }

    mPassedRule->compile(vmWriter);
}

void AlternationRule::setRuleLevel(int ruleLevel)
{
    ParentRule::setRuleLevel(ruleLevel);
    for (const auto& pRule : getChildRules())
    {
        pRule->setRuleLevel(ruleLevel);
    }
}

Rule* AlternationRule::getPassedRule()
{
    return mPassedRule.get();
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
        pRule->setParentRule(this);

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
    pRule->setParentRule(this);

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
