#pragma once

#include "BaseRules.h"

class ExpressionRule;
class TermRule;
class SubroutineCallRule;
class ExpressionListRule;
class OpRule;
class UnaryOpRule;
class KeywordConstantRule;


class ExpressionRule : public ParentRule
{
public:
    ExpressionRule();
    ~ExpressionRule() {};
};

class TermRule : public ParentRule
{
public:
    TermRule();
    ~TermRule() {};
};

class SubroutineCallRule : public ParentRule
{
public:
    SubroutineCallRule();
    ~SubroutineCallRule() {};
};

class ExpressionListRule : public ParentRule
{
public:
    ExpressionListRule();
    ~ExpressionListRule() {};
};

class OpRule : public ParentRule
{
public:
    OpRule();
    ~OpRule() {};
};

class UnaryOpRule : public ParentRule
{
public:
    UnaryOpRule();
    ~UnaryOpRule() {};
};

class KeywordConstantRule : public ParentRule
{
public:
    KeywordConstantRule();
    ~KeywordConstantRule() {};
};
