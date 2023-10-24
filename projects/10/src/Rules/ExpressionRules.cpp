#include "BaseRules.h"
#include "ExpressionRules.h"
#include "LexicalRules.h"
#include "ProgramStructureRules.h"

#pragma region ExpressionRule
ExpressionRule::ExpressionRule() : SequenceRule(
    {
        new TermRule,
        new ZeroOrMoreRule([]
        {
            return new SequenceRule(
            {
                new OpRule,
                new TermRule
            });
        })
    })
{
}

void ExpressionRule::compile()
{
    writeOutput("<expression>");
    SequenceRule::compile();
    writeOutput("</expression>");
}
#pragma endregion

#pragma region TermRule
TermRule::TermRule() : ParentRule({ })
{
    mCreateRuleFuncs =
    {
        [] { return new SubroutineCallRule; },
        [] { return new SequenceRule(
        {
            new UnaryOpRule,
            new TermRule,
        }); },
        [] { return new SequenceRule(
        {
            new SymbolRule('('),
            new ExpressionRule,
            new SymbolRule(')'),
        }); },
        [] { return new SequenceRule(
        {
            new VarNameRule,
            new SymbolRule('['),
            new ExpressionRule,
            new SymbolRule(']'),
        }); },
        [] { return new IntegerConstantRule; },
        [] { return new StringConstantRule; },
        [] { return new KeywordConstantRule; },
        [] { return new VarNameRule; }
    };
}

bool TermRule::initialize(JackTokenizer* pTokenizer)
{
    for (auto& onCreateRule : mCreateRuleFuncs)
    {
        auto pRule = onCreateRule();

        int ruleLevel = typeid(*pRule) != typeid(SequenceRule) ? mRuleLevel + 1 : mRuleLevel;
        pRule->setRuleLevel(ruleLevel);

        auto result = pRule->initialize(pTokenizer);

        if (result)
        {
            mChildRules.push_back(pRule);
            return true;
        }

        delete pRule;
    }

    return false;
}

void TermRule::compile()
{
    writeOutput("<term>");
    ParentRule::compile();
    writeOutput("</term>");
}
#pragma endregion

#pragma region SubroutineCallRule
SubroutineCallRule::SubroutineCallRule() : AlternationRule(
    {
        new SequenceRule(
        {
            new SubroutineNameRule,
            new SymbolRule('('),
            new ExpressionListRule,
            new SymbolRule(')')
        }),
        new SequenceRule(
        {
            new AlternationRule(
            {
                new ClassNameRule,
                new VarNameRule
            }),
            new SymbolRule('.'),
            new SubroutineNameRule,
            new SymbolRule('('),
            new ExpressionListRule,
            new SymbolRule(')')
        })
    })
{
}

void SubroutineCallRule::setRuleLevel(int ruleLevel)
{
    AlternationRule::setRuleLevel(ruleLevel - 1);
}
#pragma endregion

#pragma region ExpressionListRule
ExpressionListRule::ExpressionListRule() : SequenceRule(
    {
        new ZeroOrOneRule([]
        {
            return new SequenceRule(
            {
                new ExpressionRule,
                new ZeroOrMoreRule([]
                {
                    return new SequenceRule(
                    {
                        new SymbolRule(','),
                        new ExpressionRule,
                    });
                })
            });
        })
    })
{
}

void ExpressionListRule::compile()
{
    writeOutput("<expressionList>");
    SequenceRule::compile();
    writeOutput("</expressionList>");
}
#pragma endregion

#pragma region OpRule
OpRule::OpRule() : AlternationRule(
    {
        new SymbolRule('+'),
        new SymbolRule('-'),
        new SymbolRule('*'),
        new SymbolRule('/'),
        new SymbolRule('&'),
        new SymbolRule('|'),
        new SymbolRule('<'),
        new SymbolRule('>'),
        new SymbolRule('=')
    })
{
}
#pragma endregion

#pragma region UnaryOpRule
UnaryOpRule::UnaryOpRule() : AlternationRule(
    {
        new SymbolRule('-'),
        new SymbolRule('~')
    })
{
}
#pragma endregion

#pragma region KeywordConstantRule
KeywordConstantRule::KeywordConstantRule() : AlternationRule(
    {
        new KeywordRule("true"),
        new KeywordRule("false"),
        new KeywordRule("null"),
        new KeywordRule("this")
    })
{
}
#pragma endregion
