#pragma once

#include <string>
#include "BaseRules.h"


class KeywordRule;
class SymbolRule;
class IntegerConstantRule;
class StringConstantRule;
class IdentifierRule;


class KeywordRule : public Rule
{
public:
    KeywordRule(std::string keyword);
    ~KeywordRule() {};

public:
    /// <summary>
    /// Returns information if passed string is a keyword.
    /// </summary>
    static bool isKeyword(std::string name);
};

class SymbolRule : public Rule
{
public:
    SymbolRule(char symbol);
    ~SymbolRule() {};

public:
    /// <summary>
    /// Returns information if passed character is symbol.
    /// </summary>
    static bool isSymbol(char symbol);
};

class IntegerConstantRule : public Rule
{
public:
    IntegerConstantRule();
    ~IntegerConstantRule() {};
};

class StringConstantRule : public Rule
{
public:
    StringConstantRule();
    ~StringConstantRule() {};
};

class IdentifierRule : public Rule
{
public:
    IdentifierRule();
    ~IdentifierRule() {};
};
