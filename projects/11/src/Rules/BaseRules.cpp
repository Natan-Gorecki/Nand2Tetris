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

void Rule::writeXmlSyntax(ofstream* stream, int tabs)
{
    // default virtual empty method
}

void Rule::writeXmlTokens(ofstream* stream)
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

void Rule::writeXmlSyntaxImpl(ofstream* stream, int tabs, string const& text)
{
    *stream << string(tabs * 2, ' ') << text << "\n";
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

void ParentRule::writeXmlSyntax(std::ofstream* stream, int tabs)
{
    for (const auto& pRule : mChildRules)
    {
        pRule->writeXmlSyntax(stream, tabs);
    }
}

void ParentRule::writeXmlTokens(std::ofstream* stream)
{
    for (const auto& pRule : mChildRules)
    {
        pRule->writeXmlTokens(stream);
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
    ParentRule::writeXmlSyntax(stream, tabs + 1);
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

void AlternationRule::writeXmlSyntax(std::ofstream* stream, int tabs)
{
    mPassedRule->writeXmlSyntax(stream, tabs);
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
    auto ruleTabs = getChild<SequenceRule>(0) ? tabs -1 : tabs;
    ParentRule::writeXmlSyntax(stream, ruleTabs);
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

    if (pRule->initialize(pTokenizer))
    {
        getChildRules().push_back(pRule);
    }

    return true;
}

void ZeroOrOneRule::writeXmlSyntax(std::ofstream* stream, int tabs)
{
    auto ruleTabs = getChild<SequenceRule>(0) ? tabs - 1 : tabs;
    ParentRule::writeXmlSyntax(stream, ruleTabs);
}
#pragma endregion
