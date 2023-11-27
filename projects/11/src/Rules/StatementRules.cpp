#include <functional>
#include "ExpressionRules.h"
#include "LexicalRules.h"
#include "ProgramStructureRules.h"
#include "StatementRules.h"
#include "../CompilationEngine.h"

using namespace std;

#pragma region StatementsRule
StatementsRule::StatementsRule() : SequenceRule(
    {
        make_shared<ZeroOrMoreRule>([]
        {
            return make_shared<StatementRule>();
        })
    })
{
}

void StatementsRule::compile(VMWriter* vmWriter)
{
    writeOutput("<statements>");
    SequenceRule::compile(vmWriter);
    writeOutput("</statements>");
}
#pragma endregion

#pragma region StatementRule
StatementRule::StatementRule() : AlternationRule(
    {
        make_shared<LetStatementRule>(),
        make_shared<IfStatementRule>(),
        make_shared<WhileStatementRule>(),
        make_shared<DoStatementRule>(),
        make_shared<ReturnStatementRule>()
    })
{
}
#pragma endregion

#pragma region LetStatementRule
LetStatementRule::LetStatementRule() : SequenceRule(
    {
        make_shared<KeywordRule>("let"),
        make_shared<VarNameRule>(),
        make_shared<ZeroOrOneRule>([]
        {
            return make_shared<SequenceRule>(RuleVector
            {
                make_shared<SymbolRule>('['),
                make_shared<ExpressionRule>(),
                make_shared<SymbolRule>(']'),
            });
        }),
        make_shared<SymbolRule>('='),
        make_shared<ExpressionRule>(),
        make_shared<SymbolRule>(';')
    })
{
}

void LetStatementRule::compile(VMWriter* vmWriter)
{
    writeOutput("<letStatement>");
    SequenceRule::compile(vmWriter);
    writeOutput("</letStatement>");
}
#pragma endregion

#pragma region IfStatementRule
IfStatementRule::IfStatementRule() : SequenceRule(
    {
        make_shared<KeywordRule>("if"),
        make_shared<SymbolRule>('('),
        make_shared<ExpressionRule>(),
        make_shared<SymbolRule>(')'),
        make_shared<SymbolRule>('{'),
        make_shared<StatementsRule>(),
        make_shared<SymbolRule>('}'),
        make_shared<ZeroOrOneRule>([]
        {
            return make_shared<SequenceRule>(RuleVector
            {
                make_shared<KeywordRule>("else"),
                make_shared<SymbolRule>('{'),
                make_shared<StatementsRule>(),
                make_shared<SymbolRule>('}')
            });
        })
    })
{
}

void IfStatementRule::compile(VMWriter* vmWriter)
{
    writeOutput("<ifStatement>");
    SequenceRule::compile(vmWriter);
    writeOutput("</ifStatement>");
}
#pragma endregion

#pragma region WhileStatementRule
WhileStatementRule::WhileStatementRule() : SequenceRule(
    {
        make_shared<KeywordRule>("while"),
        make_shared<SymbolRule>('('),
        make_shared<ExpressionRule>(),
        make_shared<SymbolRule>(')'),
        make_shared<SymbolRule>('{'),
        make_shared<StatementsRule>(),
        make_shared<SymbolRule>('}')
    })
{
}

void WhileStatementRule::compile(VMWriter* vmWriter)
{
    writeOutput("<whileStatement>");
    SequenceRule::compile(vmWriter);
    writeOutput("</whileStatement>");
}
#pragma endregion

#pragma region DoStatementRule
DoStatementRule::DoStatementRule() : SequenceRule(
    {
        make_shared<KeywordRule>("do"),
        make_shared<SubroutineCallRule>(),
        make_shared<SymbolRule>(';')
    })
{
}

void DoStatementRule::compile(VMWriter* vmWriter)
{
    writeOutput("<doStatement>");
    SequenceRule::compile(vmWriter);
    writeOutput("</doStatement>");
}
#pragma endregion

#pragma region ReturnStatementRule
ReturnStatementRule::ReturnStatementRule() : SequenceRule(
    {
        make_shared<KeywordRule>("return"),
        make_shared<ZeroOrOneRule>([]
        {
            return make_shared<ExpressionRule>();
        }),
        make_shared<SymbolRule>(';')
    })
{
}

void ReturnStatementRule::compile(VMWriter* vmWriter)
{
    writeOutput("<returnStatement>");
    SequenceRule::compile(vmWriter);
    writeOutput("</returnStatement>");
}
#pragma endregion
