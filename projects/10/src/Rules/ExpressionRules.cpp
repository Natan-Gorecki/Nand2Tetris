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
