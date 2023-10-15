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
    virtual ~ExpressionRule() {};
};

class TermRule : public ParentRule
{
public:
    TermRule();
    virtual ~TermRule() {};
};

class SubroutineCallRule : public ParentRule
{
public:
    SubroutineCallRule();
    virtual ~SubroutineCallRule() {};
};

class ExpressionListRule : public ParentRule
{
public:
    ExpressionListRule();
    virtual ~ExpressionListRule() {};
};

class OpRule : public ParentRule
{
public:
    OpRule();
    virtual ~OpRule() {};
};

class UnaryOpRule : public ParentRule
{
public:
    UnaryOpRule();
    virtual ~UnaryOpRule() {};
};

class KeywordConstantRule : public ParentRule
{
public:
    KeywordConstantRule();
    virtual ~KeywordConstantRule() {};
};
