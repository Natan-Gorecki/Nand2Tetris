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
};

class SymbolRule : public Rule
{
public:
    SymbolRule(char symbol);
    ~SymbolRule() {};
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
