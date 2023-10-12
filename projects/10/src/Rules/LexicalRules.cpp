#include <set>
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

/// <summary>
/// Returns information if passed character is symbol.
/// </summary>
bool SymbolRule::isSymbol(char symbol)
{
    return symbols.count(symbol) != 0;
}

/// <summary>
/// Returns information if passed string is a keyword.
/// </summary>
bool KeywordRule::isKeyword(std::string name)
{
    return keywords.count(name) != 0;
}
