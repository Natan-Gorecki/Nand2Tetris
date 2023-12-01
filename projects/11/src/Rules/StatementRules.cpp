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

void StatementsRule::writeXmlSyntax(std::ofstream* stream)
{
    writeXmlSyntaxImpl(stream, "<statements>");
    SequenceRule::writeXmlSyntax(stream);
    writeXmlSyntaxImpl(stream, "</statements>");
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

void LetStatementRule::writeXmlSyntax(std::ofstream* stream)
{
    writeXmlSyntaxImpl(stream, "<letStatement>");
    SequenceRule::writeXmlSyntax(stream);
    writeXmlSyntaxImpl(stream, "</letStatement>");
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

void IfStatementRule::writeXmlSyntax(std::ofstream* stream)
{
    writeXmlSyntaxImpl(stream, "<ifStatement>");
    SequenceRule::writeXmlSyntax(stream);
    writeXmlSyntaxImpl(stream, "</ifStatement>");
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

void WhileStatementRule::writeXmlSyntax(std::ofstream* stream)
{
    writeXmlSyntaxImpl(stream, "<whileStatement>");
    SequenceRule::writeXmlSyntax(stream);
    writeXmlSyntaxImpl(stream, "</whileStatement>");
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
    SequenceRule::compile(vmWriter);
    vmWriter->writePop(ESegment::TEMP, 0);
}

void DoStatementRule::writeXmlSyntax(std::ofstream* stream)
{
    writeXmlSyntaxImpl(stream, "<doStatement>");
    SequenceRule::writeXmlSyntax(stream);
    writeXmlSyntaxImpl(stream, "</doStatement>");
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

void ReturnStatementRule::writeXmlSyntax(std::ofstream* stream)
{
    writeXmlSyntaxImpl(stream, "<returnStatement>");
    SequenceRule::writeXmlSyntax(stream);
    writeXmlSyntaxImpl(stream, "</returnStatement>");
}
#pragma endregion
