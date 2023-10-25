#include <set>
#include <stdexcept>
#include "LexicalRules.h"
#include "../CompilationEngine.h"
#include "../JackAnalyzerError.h"

using namespace std;

const set<string, less<>> keywords =
{
    "class",
    "constructor", "function", "method",
    "field", "static", "var",
    "int", "char", "boolean",
    "void",
    "true", "false",
    "null",
    "this",
    "let", "do", "if", "else", "while", "return"
};

const set<char> symbols =
{
    '{', '}',
    '(', ')',
    '[', ']',
    '.', ',', ';',
    '+', '-', '*', '/',
    '&', '|',
    '<', '>', '=',
    '~'
};

#pragma region LexicalRule
bool LexicalRule::initialize(JackTokenizer* pTokenizer)
{
    if (!pTokenizer->advance())
    {
        return false;
    }

    auto result = isFullfiled(pTokenizer);

    if (!result)
    {
        pTokenizer->reverse();
    }

    return result;
}

void LexicalRule::compile()
{
    writeOutput(toString());
    writeToken(toString());
}
#pragma endregion

#pragma region KeywordRule
/// <summary>
/// Returns information if passed string is a keyword.
/// </summary>
bool KeywordRule::isKeyword(string const& name)
{
    return keywords.count(name) != 0;
}

KeywordRule::KeywordRule(string const& keyword)
{
    if (!isKeyword(keyword))
    {
        throw JackAnalyzerError("Invalid keyword: " + keyword);
    }

    mKeyword = keyword;
}

bool KeywordRule::isFullfiled(JackTokenizer* pTokenizer)
{
    if (pTokenizer->tokenType() == ETokenType::KEYWORD && pTokenizer->keyword() == mKeyword)
    {
        return true;
    }

    return false;
}

string KeywordRule::toString()
{
    return "<keyword> " + mKeyword + " </keyword>";
}
#pragma endregion

#pragma region SymbolRule
/// <summary>
/// Returns information if passed character is symbol.
/// </summary>
bool SymbolRule::isSymbol(char symbol)
{
    return symbols.count(symbol) != 0;
}

SymbolRule::SymbolRule(char symbol)
{
    if (!isSymbol(symbol))
    {
        throw JackAnalyzerError("Invalid symbol: " + symbol);
    }

    mSymbol = symbol;
}

bool SymbolRule::isFullfiled(JackTokenizer* pTokenizer)
{
    if (pTokenizer->tokenType() == ETokenType::SYMBOL && pTokenizer->symbol() == mSymbol)
    {
        return true;
    }

    return false;
}

string SymbolRule::toString()
{
    return "<symbol> " + encodeXmlSymbol(mSymbol) + " </symbol>";
}

string SymbolRule::encodeXmlSymbol(char symbol) const
{
    switch (symbol)
    {
    case '<':
        return "&lt;";
    case '>':
        return "&gt;";
    case '"':
        return "&quot;";
    case '&':
        return "&amp;";
    default:
        return string(1, symbol);
    }
}
#pragma endregion

#pragma region IntegerConstantRule
bool IntegerConstantRule::isFullfiled(JackTokenizer* pTokenizer)
{
    if (pTokenizer->tokenType() == ETokenType::INT_CONST)
    {
        mIntVal = pTokenizer->intVal();
        return true;
    }

    return false;
}

string IntegerConstantRule::toString()
{
    return "<integerConstant> " + to_string(mIntVal) + " </integerConstant>";
}
#pragma endregion

#pragma region StringConstantRule
bool StringConstantRule::isFullfiled(JackTokenizer* pTokenizer)
{
    if (pTokenizer->tokenType() == ETokenType::STRING_CONST)
    {
        mStringVal = pTokenizer->stringVal();
        return true;
    }

    return false;
}

string StringConstantRule::toString()
{
    return "<stringConstant> " + mStringVal + " </stringConstant>";
}
#pragma endregion

#pragma region IdentifierRule
bool IdentifierRule::isFullfiled(JackTokenizer* pTokenizer)
{
    if (pTokenizer->tokenType() == ETokenType::IDENTIFIER)
    {
        mIdentifier = pTokenizer->identifier();
        return true;
    }

    return false;
}

string IdentifierRule::toString()
{
    return "<identifier> " + mIdentifier + " </identifier>";
}
#pragma endregion
