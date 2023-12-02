#include <functional>
#include "ExpressionRules.h"
#include "LexicalRules.h"
#include "ProgramStructureRules.h"
#include "RuleUtils.h"
#include "StatementRules.h"
#include "../CompilationEngine.h"
#include "../JackCompilerError.h"

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

void StatementsRule::writeXmlSyntax(std::ofstream* stream, int tabs)
{
    XML_SYNTAX("<statements>");
    SequenceRule::writeXmlSyntax(stream, tabs);
    XML_SYNTAX("</statements>");
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
    getChild(4)->compile(vmWriter);
    auto variableName = getChild(1)->cast<VarNameRule>()->toString();
    auto symbol = RuleUtils::findSymbol(this, variableName);

    if (getChild(2)->getChildRules().empty())
    {
        vmWriter->writePop(symbol.getSegment(), symbol.index);
        return;
    }

    vmWriter->writePush(symbol.getSegment(), symbol.index);
    getChild(2)->getChild(0)->getChild(1)->compile(vmWriter);
    vmWriter->writeArithmetic(EArithmetic::ADD);
    vmWriter->writePop(ESegment::POINTER, 1);
    vmWriter->writePop(ESegment::THAT, 0);
}

void LetStatementRule::writeXmlSyntax(std::ofstream* stream, int tabs)
{
    XML_SYNTAX("<letStatement>");
    SequenceRule::writeXmlSyntax(stream, tabs);
    XML_SYNTAX("</letStatement>");
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
    auto uniqueNumber = getParentRecursive<SubroutineDecRule>()->getUniqueNumber(this);

    getChild(2)->compile(vmWriter);
    vmWriter->writeIf("IF_TRUE" + to_string(uniqueNumber));
    vmWriter->writeGoto("IF_FALSE" + to_string(uniqueNumber));

    vmWriter->writeLabel("IF_TRUE" + to_string(uniqueNumber));
    getChild(5)->compile(vmWriter);
    vmWriter->writeGoto("IF_END" + to_string(uniqueNumber));

    vmWriter->writeLabel("IF_FALSE" + to_string(uniqueNumber));
    getChild(7)->compile(vmWriter);

    vmWriter->writeLabel("IF_END" + to_string(uniqueNumber));
}

void IfStatementRule::writeXmlSyntax(std::ofstream* stream, int tabs)
{
    XML_SYNTAX("<ifStatement>");
    SequenceRule::writeXmlSyntax(stream, tabs);
    XML_SYNTAX("</ifStatement>");
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
    auto uniqueNumber = getParentRecursive<SubroutineDecRule>()->getUniqueNumber(this);

    vmWriter->writeLabel("WHILE_EXP" + to_string(uniqueNumber));
    getChild(2)->compile(vmWriter);
    
    vmWriter->writeArithmetic(EArithmetic::NOT);
    vmWriter->writeIf("WHILE_END" + to_string(uniqueNumber));

    getChild(5)->compile(vmWriter);
    vmWriter->writeGoto("WHILE_EXP" + to_string(uniqueNumber));

    vmWriter->writeLabel("WHILE_END" + to_string(uniqueNumber));
}

void WhileStatementRule::writeXmlSyntax(std::ofstream* stream, int tabs)
{
    XML_SYNTAX("<whileStatement>");
    SequenceRule::writeXmlSyntax(stream, tabs);
    XML_SYNTAX("</whileStatement>");
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

void DoStatementRule::writeXmlSyntax(std::ofstream* stream, int tabs)
{
    XML_SYNTAX("<doStatement>");
    SequenceRule::writeXmlSyntax(stream, tabs);
    XML_SYNTAX("</doStatement>");
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
    if (getChild(1)->getChildRules().empty())
    {
        // void method are handled by SubroutineDecRule
        return;
    }

    SequenceRule::compile(vmWriter);
    vmWriter->writeReturn();
}

void ReturnStatementRule::writeXmlSyntax(std::ofstream* stream, int tabs)
{
    XML_SYNTAX("<returnStatement>");
    SequenceRule::writeXmlSyntax(stream, tabs);
    XML_SYNTAX("</returnStatement>");
}
#pragma endregion
