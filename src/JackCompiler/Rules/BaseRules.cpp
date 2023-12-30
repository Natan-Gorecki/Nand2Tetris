#include <typeinfo>
#include "BaseRules.h"
#include "../CompilationEngine.h"
#include "../JackCompilerError.h"

using namespace std;

#pragma region Rule
Rule::Rule(RuleVector const& rules)
{
    for (const auto& pRule : rules)
    {
        mChildRules.push_back(pRule);
    }
}

bool Rule::initialize(JackTokenizer* pTokenizer)
{
    for (const auto& pRule : mChildRules)
    {
        pRule->setParent(this);
    }
    return true;
}

void Rule::compile(VMWriter* vmWriter)
{
    for (const auto& pRule : mChildRules)
    {
        pRule->compile(vmWriter);
    }
}

void Rule::writeXmlSyntax(ofstream* stream, int tabs)
{
    for (const auto& pRule : mChildRules)
    {
        pRule->writeXmlSyntax(stream, tabs);
    }
}

void Rule::writeXmlTokens(ofstream* stream)
{
    for (const auto& pRule : mChildRules)
    {
        pRule->writeXmlTokens(stream);
    }
}

Rule* Rule::getParent()
{
    return mParentRule;
}

void Rule::setParent(Rule* pRule)
{
    mParentRule = pRule;
}

Rule* Rule::getChild(int index)
{
    if (index >= mChildRules.size())
    {
        return nullptr;
    }
    return mChildRules[index].get();
}

RuleVector& Rule::getChildRules()
{
    return mChildRules;
}
#pragma endregion

#pragma region SequenceRule
SequenceRule::SequenceRule(RuleVector const& rules)
    : Rule(rules)
{
}

bool SequenceRule::initialize(JackTokenizer* pTokenizer)
{
    Rule::initialize(pTokenizer);

    auto initialPosition = pTokenizer->getPosition();
    for (const auto& pRule : getChildRules())
    {
        auto result = pRule->initialize(pTokenizer);
        
        if (!result)
        {
            pTokenizer->setPosition(initialPosition);
            return false;
        }
    }

    return true;
}

void SequenceRule::writeXmlSyntax(std::ofstream* stream, int tabs)
{
    Rule::writeXmlSyntax(stream, tabs + 1);
}
#pragma endregion

#pragma region AlternationRule
AlternationRule::AlternationRule(RuleVector const& rules)
    : Rule(rules)
{
}

bool AlternationRule::initialize(JackTokenizer* pTokenizer)
{
    Rule::initialize(pTokenizer);

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

void AlternationRule::writeXmlSyntax(std::ofstream* stream, int tabs)
{
    mPassedRule->writeXmlSyntax(stream, tabs);
}

Rule* AlternationRule::getTrueRule()
{
    return mPassedRule.get();
}
#pragma endregion

#pragma region ZeroOrMoreRule
ZeroOrMoreRule::ZeroOrMoreRule(CreateRuleFunc const& createRuleFunc)
    : Rule({ }),
    onCreateRule(createRuleFunc)
{
}

bool ZeroOrMoreRule::initialize(JackTokenizer* pTokenizer)
{
    bool result = true;
    while (result)
    {
        auto pRule = onCreateRule();
        pRule->setParent(this);

        if (!pRule->initialize(pTokenizer))
        {
            break;
        }

        getChildRules().push_back(pRule);
    }

    return true;
}

void ZeroOrMoreRule::writeXmlSyntax(std::ofstream* stream, int tabs)
{
    int ruleTabs = tabs;
    if (getChildRules().size() > 0)
    {
        const Rule& rule = *getChild(0);
        if (typeid(rule) == typeid(SequenceRule))
        {
            ruleTabs = tabs - 1;
        }
    }
    Rule::writeXmlSyntax(stream, ruleTabs);
}
#pragma endregion

#pragma region ZeroOrOneRule
ZeroOrOneRule::ZeroOrOneRule(CreateRuleFunc const& createRuleFunc)
    : Rule({ }),
    onCreateRule(createRuleFunc)
{
}

bool ZeroOrOneRule::initialize(JackTokenizer* pTokenizer)
{
    auto pRule = onCreateRule();
    pRule->setParent(this);

    if (pRule->initialize(pTokenizer))
    {
        getChildRules().push_back(pRule);
    }

    return true;
}

void ZeroOrOneRule::writeXmlSyntax(std::ofstream* stream, int tabs)
{
    int ruleTabs = tabs;
    if (getChildRules().size() > 0)
    {
        const Rule& rule = *getChild(0);
        if (typeid(rule) == typeid(SequenceRule))
        {
            ruleTabs = tabs - 1;
        }
    }
    Rule::writeXmlSyntax(stream, ruleTabs);
}
#pragma endregion
