#include <functional>
#include "ExpressionRules.h"
#include "LexicalRules.h"
#include "ProgramStructureRules.h"
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

    auto& classTable = getParentRecursive<ClassRule>()->getSymbolTable();
    auto& subroutineTable = getParentRecursive<SubroutineDecRule>()->getSymbolTable();

    ESymbolKind variableKind = subroutineTable.kindOf(variableName);
    int variableIndex = subroutineTable.indexOf(variableName);
    if (variableKind == ESymbolKind::UNDEFINED)
    {
        variableKind = classTable.kindOf(variableName);
        variableIndex = classTable.indexOf(variableName);
    }
    if (variableKind == ESymbolKind::UNDEFINED)
    {
        throw JackCompilerError("Could not find " + variableName + " variable.");
    }

    if (getChild(2)->getChildRules().empty())
    {
        vmWriter->writePop(symbolKindToSegment(variableKind), variableIndex);
        return;
    }

    vmWriter->writePush(symbolKindToSegment(variableKind), variableIndex);
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

ESegment LetStatementRule::symbolKindToSegment(ESymbolKind symbolKind)
{
    switch (symbolKind)
    {
    case ESymbolKind::STATIC:
        return ESegment::STATIC;
    case ESymbolKind::FIELD:
        return ESegment::THIS;
    case ESymbolKind::ARG:
        return ESegment::ARGUMENT;
    case ESymbolKind::VAR:
        return ESegment::LOCAL;
    case ESymbolKind::UNDEFINED:
    default:
        throw JackCompilerError("Symbol kind is undefined.");
    }
}
#pragma endregion

#pragma region IfStatementRule
int IfStatementRule::unique = 0;

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
    int uniqueNumber = unique++;

    getChild(2)->compile(vmWriter);
    vmWriter->writeArithmetic(EArithmetic::NOT);
    vmWriter->writeIf("IF_TRUE" + uniqueNumber);
    
    getChild(7)->compile(vmWriter);
    vmWriter->writeGoto("IF_FALSE" + uniqueNumber);
    
    vmWriter->writeLabel("IF_TRUE" + uniqueNumber);
    getChild(5)->compile(vmWriter);

    vmWriter->writeLabel("IF_FALSE" + uniqueNumber);
}

void IfStatementRule::writeXmlSyntax(std::ofstream* stream, int tabs)
{
    XML_SYNTAX("<ifStatement>");
    SequenceRule::writeXmlSyntax(stream, tabs);
    XML_SYNTAX("</ifStatement>");
}
#pragma endregion

#pragma region WhileStatementRule
int WhileStatementRule::unique = 0;

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
    int uniqueNumber = unique++;

    vmWriter->writeLabel("WHILE_EXP" + uniqueNumber);
    getChild(2)->compile(vmWriter);
    
    vmWriter->writeArithmetic(EArithmetic::NOT);
    vmWriter->writeIf("WHILE_END" + uniqueNumber);

    getChild(5)->compile(vmWriter);
    vmWriter->writeGoto("WHILE_EXP" + uniqueNumber);

    vmWriter->writeLabel("WHILE_END" + uniqueNumber);
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
