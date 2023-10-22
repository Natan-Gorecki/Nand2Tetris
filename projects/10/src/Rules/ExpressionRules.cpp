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
TermRule::TermRule() : AlternationRule(
    {
        new IntegerConstantRule,
        new StringConstantRule,
        new KeywordConstantRule,
        new VarNameRule,
        /*new SequenceRule(
        {
            new VarNameRule,
            new SymbolRule('['),
            new ExpressionRule,
            new SymbolRule(']'),
        }),*/
        /*new SequenceRule(
        {
            new SymbolRule('('),
            new ExpressionRule,
            new SymbolRule(')'),
        }),*/
        /*new SequenceRule(
        {
            new UnaryOpRule,
            new TermRule,
        }),*/
        new SubroutineCallRule
    })
{
}

//bool TermRule::initialize(JackTokenizer* pTokenizer)
//{
//    while ()
//    for (Rule* pRule : mChildRules)
//    {
//        auto result = pRule->initialize(pTokenizer);
//
//        if (result)
//        {
//            mCompileRule = pRule;
//            return true;
//        }
//    }
//
//    return false;
//}
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
#pragma endregion

#pragma region ExpressionListRule
ExpressionListRule::ExpressionListRule() : ZeroOrOneRule([]
    {
        return new SequenceRule(
        {
            new ZeroOrOneRule([]
            {
                return new IdentifierRule;
            })
            /*new ExpressionRule,
            new ZeroOrMoreRule([]
            {
                return new SequenceRule(
                {
                    new SymbolRule(','),
                    new ExpressionRule,
                });
            })*/
        });
    })
{
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
