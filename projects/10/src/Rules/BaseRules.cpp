#include "BaseRules.h"

#pragma region Rule
void Rule::setOutputFunc(StreamWriteFunc pOutputFunc)
{
    Rule::onWriteOutput = pOutputFunc;
}

void Rule::setTokensFunc(StreamWriteFunc pTokenFunc)
{
    Rule::onWriteToken = pTokenFunc;
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
#pragma endregion

#pragma region SequenceRule
SequenceRule::SequenceRule(RuleVector rules) 
    : ParentRule(rules)
{
}

bool SequenceRule::compile(JackTokenizer* pTokenizer)
{
    for (Rule* pRule : mChildRules)
    {
        if (!pRule->compile(pTokenizer))
        {
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

bool AlternationRule::compile(JackTokenizer* pTokenizer)
{
    for (Rule* pRule : mChildRules)
    {
        if (pRule->compile(pTokenizer))
        {
            return true;
        }
    }

    return false;
}
#pragma endregion

#pragma region ZeroOrMoreRule
ZeroOrMoreRule::ZeroOrMoreRule(RuleVector rules)
    : ParentRule({ new SequenceRule(rules) })
{
}

bool ZeroOrMoreRule::compile(JackTokenizer* pTokenizer)
{
    bool compileResult = true;
    while (compileResult)
    {
        compileResult = mChildRules[0]->compile(pTokenizer);
    }
}
#pragma endregion

#pragma region ZeroOrOneRule
ZeroOrOneRule::ZeroOrOneRule(RuleVector rules)
    : ParentRule({ new SequenceRule(rules) })
{
}

bool ZeroOrOneRule::compile(JackTokenizer* pTokenizer)
{
    mChildRules[0]->compile(pTokenizer);
}
#pragma endregion
