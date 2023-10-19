#include "BaseRules.h"
#include "../CompilationEngine.h"

#pragma region Rule
CompileResult Rule::compile(JackTokenizer* pTokenizer, bool advanceToken)
{
    beforeCompile(pTokenizer, advanceToken);
    bool compileResult = doCompile(pTokenizer);
    return afterCompile(pTokenizer, compileResult);
}

void Rule::beforeCompile(JackTokenizer* pTokenizer, bool advanceToken)
{
    if (advanceToken)
    {
        CompilationEngine::advanceToken();
    }
}

bool Rule::doCompile(JackTokenizer* pTokenizer)
{
    return true;
}

CompileResult Rule::afterCompile(JackTokenizer* pTokenizer, bool compileResult)
{
    return { compileResult, true };
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

bool SequenceRule::doCompile(JackTokenizer* pTokenizer)
{
    bool advanceToken = false;
    for (Rule* pRule : mChildRules)
    {
        auto compileResult = pRule->compile(pTokenizer, advanceToken);
        advanceToken = compileResult.tokenConsumed;

        if (!compileResult.compileResult)
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

bool AlternationRule::doCompile(JackTokenizer* pTokenizer)
{
    for (Rule* pRule : mChildRules)
    {
        auto compileResult = pRule->compile(pTokenizer, false);

        if (compileResult.compileResult)
        {
            return true;
        }
    }

    return false;
}
#pragma endregion

#pragma region ZeroOrMoreRule
ZeroOrMoreRule::ZeroOrMoreRule(RuleVector rules)
    : ParentRule(rules)
{
}

bool ZeroOrMoreRule::doCompile(JackTokenizer* pTokenizer)
{
    CompileResult compileResult = { true, false };
    
    while (compileResult.compileResult)
    {
        for (int i = 0; i < mChildRules.size(); i++)
        {
            compileResult = mChildRules[i]->compile(pTokenizer, compileResult.tokenConsumed);

            if (!compileResult.compileResult && i > 0)
            {
                throw std::runtime_error("ZeroOrMoreRule failed - tokenizer advanced without going back.");
            }

            if (!compileResult.compileResult)
            {
                break;
            }
        }
    }

    return true;
}

CompileResult ZeroOrMoreRule::afterCompile(JackTokenizer* pTokenizer, bool compileResult)
{
    return { compileResult, false };
}
#pragma endregion

#pragma region ZeroOrOneRule
ZeroOrOneRule::ZeroOrOneRule(RuleVector rules)
    : ParentRule(rules)
{
}

bool ZeroOrOneRule::doCompile(JackTokenizer* pTokenizer)
{
    CompileResult compileResult = { false, false };

    for (int i = 0; i < mChildRules.size(); i++)
    {
        compileResult = mChildRules[i]->compile(pTokenizer, compileResult.tokenConsumed);

        if (!compileResult.compileResult && i > 0)
        {
            throw std::runtime_error("ZeroOrOneRule failed - tokenizer advanced without going back.");
        }

        if (!compileResult.compileResult)
        {
            break;
        }
    }

    return compileResult.compileResult;
}

CompileResult ZeroOrOneRule::afterCompile(JackTokenizer* pTokenizer, bool compileResult)
{
    return { true, compileResult };
}
#pragma endregion
