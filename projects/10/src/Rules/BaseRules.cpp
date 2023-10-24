#include <typeinfo>
#include "BaseRules.h"
#include "../CompilationEngine.h"

#pragma region Rule
bool Rule::initialize(JackTokenizer* pTokenizer)
{
    return true;
}

void Rule::compile()
{

}

void Rule::setRuleLevel(int ruleLevel)
{
    mRuleLevel = ruleLevel;
}

void Rule::writeOutput(std::string text)
{
    std::string tabs = std::string(mRuleLevel * 2, ' ');
    CompilationEngine::writeOutput(tabs + text + "\n");
}

void Rule::writeToken(std::string text)
{
    CompilationEngine::writeToken(text + "\n");
}
#pragma endregion

#pragma region ParentRule
ParentRule::ParentRule(RuleVector rules)
{
    for (Rule* rule : rules)
    {
        mChildRules.push_back(rule);
    }
}

ParentRule::~ParentRule()
{
    for (Rule* rule : mChildRules)
    {
        delete rule;
    }
}

void ParentRule::compile()
{
    for (Rule* pRule : mChildRules)
    {
        pRule->compile();
    }
}
#pragma endregion

#pragma region SequenceRule
SequenceRule::SequenceRule(RuleVector rules)
    : ParentRule(rules)
{
}

bool SequenceRule::initialize(JackTokenizer* pTokenizer)
{
    auto initialPosition = pTokenizer->getPosition();

    for (Rule* pRule : mChildRules)
    {
        pRule->setRuleLevel(mRuleLevel + 1);
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
AlternationRule::AlternationRule(RuleVector rules)
    : ParentRule(rules)
{
}

bool AlternationRule::initialize(JackTokenizer* pTokenizer)
{
    for (Rule* pRule : mChildRules)
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
        throw std::runtime_error("Failed to compile AlternationRule.");
    }

    mCompileRule->compile();
}

void AlternationRule::setRuleLevel(int ruleLevel)
{
    mRuleLevel = ruleLevel;
    for (Rule* pRule : mChildRules)
    {
        pRule->setRuleLevel(ruleLevel);
    }
}
#pragma endregion

#pragma region ZeroOrMoreRule
ZeroOrMoreRule::ZeroOrMoreRule(CreateRuleFunc createRuleFunc)
    : ParentRule({ })
{
    onCreateRule = createRuleFunc;
}

bool ZeroOrMoreRule::initialize(JackTokenizer* pTokenizer)
{
    bool result = true;

    while (result)
    {
        auto pRule = onCreateRule();

        int ruleLevel = typeid(*pRule) != typeid(SequenceRule) ? mRuleLevel : mRuleLevel - 1;
        pRule->setRuleLevel(ruleLevel);

        auto initializeResult = pRule->initialize(pTokenizer);

        if (!initializeResult)
        {
            delete pRule;
            break;
        }

        mChildRules.push_back(pRule);
    }

    return true;
}
#pragma endregion

#pragma region ZeroOrOneRule
ZeroOrOneRule::ZeroOrOneRule(CreateRuleFunc createRuleFunc)
    : ParentRule({ })
{
    onCreateRule = createRuleFunc;
}

bool ZeroOrOneRule::initialize(JackTokenizer* pTokenizer)
{
    auto pRule = onCreateRule();

    int ruleLevel = typeid(*pRule) != typeid(SequenceRule) ? mRuleLevel : mRuleLevel - 1;
    pRule->setRuleLevel(ruleLevel);

    auto initializeResult = pRule->initialize(pTokenizer);

    if (!initializeResult)
    {
        delete pRule;
        return true;
    }

    mChildRules.push_back(pRule);
    return true;
}
#pragma endregion
