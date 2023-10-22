#pragma once

#include "BaseRules.h"

class ExpressionRule;
class TermRule;
class SubroutineCallRule;
class ExpressionListRule;
class OpRule;
class UnaryOpRule;
class KeywordConstantRule;


class ExpressionRule : public SequenceRule
{
public:
    ExpressionRule();
    virtual ~ExpressionRule() {};

public:
    void compile() override;
};

class TermRule : public AlternationRule
{
public:
    TermRule();
    virtual ~TermRule() {};
};

class SubroutineCallRule : public AlternationRule
{
public:
    SubroutineCallRule();
    virtual ~SubroutineCallRule() {};
};

class ExpressionListRule : public ZeroOrOneRule
{
public:
    ExpressionListRule();
    virtual ~ExpressionListRule() {};
};

class OpRule : public AlternationRule
{
public:
    OpRule();
    virtual ~OpRule() {};
};

class UnaryOpRule : public AlternationRule
{
public:
    UnaryOpRule();
    virtual ~UnaryOpRule() {};
};

class KeywordConstantRule : public AlternationRule
{
public:
    KeywordConstantRule();
    virtual ~KeywordConstantRule() {};
};
