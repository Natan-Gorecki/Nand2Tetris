#include <set>
#include <stdexcept>
#include "LexicalRules.h"

std::set<std::string> keywords =
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

std::set<char> symbols =
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
bool LexicalRule::compile(JackTokenizer* pTokenizer)
{
    bool result = lexicalCompile(pTokenizer);
    if (result)
    {
        onWriteOutput(toString(pTokenizer));
        onWriteToken(toString(pTokenizer));
    }
}
#pragma endregion

#pragma region KeywordRule
/// <summary>
/// Returns information if passed string is a keyword.
/// </summary>
bool KeywordRule::isKeyword(std::string name)
{
    return keywords.count(name) != 0;
}

KeywordRule::KeywordRule(std::string keyword)
{
    if (!isKeyword(keyword))
    {
        throw std::runtime_error("Invalid keyword: " + keyword);
    }

    mKeyword = keyword;
}

bool KeywordRule::lexicalCompile(JackTokenizer* pTokenizer)
{
    if (pTokenizer->tokenType() == ETokenType::KEYWORD && pTokenizer->keyword() == mKeyword)
    {
        return true;
    }

    return false;
}

std::string KeywordRule::toString(JackTokenizer* pTokenizer)
{
    return "<keyword> " + pTokenizer->keyword() + " </keyword>\n";
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
        throw std::runtime_error("Invalid symbol: " + symbol);
    }

    mSymbol = symbol;
}

bool SymbolRule::lexicalCompile(JackTokenizer* pTokenizer)
{
    if (pTokenizer->tokenType() == ETokenType::SYMBOL && pTokenizer->symbol() == mSymbol)
    {
        return true;
    }

    return false;
}

std::string SymbolRule::toString(JackTokenizer* pTokenizer)
{
    return "<symbol> " + encodeXmlSymbol(pTokenizer->symbol()) + " </symbol>\n";
}

std::string SymbolRule::encodeXmlSymbol(char symbol)
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
        return std::string(1, symbol);
    }
}
#pragma endregion

#pragma region IntegerConstantRule
bool IntegerConstantRule::lexicalCompile(JackTokenizer* pTokenizer)
{
    return pTokenizer->tokenType() == ETokenType::INT_CONST;
}

std::string IntegerConstantRule::toString(JackTokenizer* pTokenizer)
{
    return "<integerConstant> " + std::to_string(pTokenizer->intVal()) + " </integerConstant>\n";
}
#pragma endregion

#pragma region StringConstantRule
bool StringConstantRule::lexicalCompile(JackTokenizer* pTokenizer)
{
    return pTokenizer->tokenType() == ETokenType::STRING_CONST;
}

std::string StringConstantRule::toString(JackTokenizer* pTokenizer)
{
    return "<stringConstant> " + pTokenizer->stringVal() + " </stringConstant>\n";
}
#pragma endregion

#pragma region IdentifierRule
bool IdentifierRule::lexicalCompile(JackTokenizer* pTokenizer)
{
    return pTokenizer->tokenType() == ETokenType::IDENTIFIER;
}

std::string IdentifierRule::toString(JackTokenizer* pTokenizer)
{
    return "<identifier> " + pTokenizer->identifier() + " </identifier>\n";
}
#pragma endregion
