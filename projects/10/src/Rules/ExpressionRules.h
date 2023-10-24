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

class TermRule : public ParentRule
{
public:
    TermRule();
    virtual ~TermRule() {};

public:
    bool initialize(JackTokenizer* pTokenizer) override;
    void compile() override;

private:
    std::vector<CreateRuleFunc> mCreateRuleFuncs;
};

class SubroutineCallRule : public AlternationRule
{
public:
    SubroutineCallRule();
    virtual ~SubroutineCallRule() {};

public:
    void setRuleLevel(int ruleLevel) override;
};

class ExpressionListRule : public SequenceRule
{
public:
    ExpressionListRule();
    virtual ~ExpressionListRule() {};

public:
    void compile() override;
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
