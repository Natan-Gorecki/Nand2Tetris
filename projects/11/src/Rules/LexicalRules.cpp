#include <algorithm>
#include <set>
#include <stdexcept>
#include "LexicalRules.h"
#include "../CompilationEngine.h"
#include "../JackCompilerError.h"

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
    string ruleName = typeid(this).name();
    string suffix = "Rule";

    // remove Rule suffix
    ruleName = ruleName.substr(0, ruleName.length() - suffix.length());

    // to lowercase
    std::transform(ruleName.begin(), ruleName.end(), ruleName.begin(), [](unsigned char c) { return std::tolower(c); });

    string toWrite = "<" + ruleName + ">" + encodeXml(toString()) + "</" + ruleName + ">";

    writeOutput(toWrite);
    writeToken(toWrite);
}

string LexicalRule::encodeXml(string text) const
{
    string result;

    for (char symbol : text)
    {
        switch (symbol)
        {
        case '<':
            result += "&lt;";
            continue;
        case '>':
            result += "&gt;";
            continue;
        case '"':
            result += "&quot;";
            continue;
        case '&':
            result += "&amp;";
            continue;
        default:
            result += symbol;
            continue;
        }
    }

    return result;
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
        throw JackCompilerError("Invalid keyword: " + keyword);
    }

    mKeyword = keyword;
}

string KeywordRule::toString()
{
    return mKeyword;
}

bool KeywordRule::isFullfiled(JackTokenizer* pTokenizer)
{
    if (pTokenizer->tokenType() == ETokenType::KEYWORD && pTokenizer->keyword() == mKeyword)
    {
        return true;
    }

    return false;
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
        throw JackCompilerError("Invalid symbol: " + symbol);
    }

    mSymbol = symbol;
}

string SymbolRule::toString()
{
    return string(1, mSymbol);
}

bool SymbolRule::isFullfiled(JackTokenizer* pTokenizer)
{
    if (pTokenizer->tokenType() == ETokenType::SYMBOL && pTokenizer->symbol() == mSymbol)
    {
        return true;
    }

    return false;
}
#pragma endregion

#pragma region IntegerConstantRule
string IntegerConstantRule::toString()
{
    return to_string(mIntVal);
}

bool IntegerConstantRule::isFullfiled(JackTokenizer* pTokenizer)
{
    if (pTokenizer->tokenType() == ETokenType::INT_CONST)
    {
        mIntVal = pTokenizer->intVal();
        return true;
    }

    return false;
}
#pragma endregion

#pragma region StringConstantRule
string StringConstantRule::toString()
{
    return mStringVal;
}

bool StringConstantRule::isFullfiled(JackTokenizer* pTokenizer)
{
    if (pTokenizer->tokenType() == ETokenType::STRING_CONST)
    {
        mStringVal = pTokenizer->stringVal();
        return true;
    }

    return false;
}
#pragma endregion

#pragma region IdentifierRule
string IdentifierRule::toString()
{
    return mIdentifier;
}

bool IdentifierRule::isFullfiled(JackTokenizer* pTokenizer)
{
    if (pTokenizer->tokenType() == ETokenType::IDENTIFIER)
    {
        mIdentifier = pTokenizer->identifier();
        return true;
    }

    return false;
}
#pragma endregion
