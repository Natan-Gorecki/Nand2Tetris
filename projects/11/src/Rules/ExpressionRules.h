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
    ~ExpressionRule() override = default;

    void compile(VMWriter* vmWriter) override;
};

class TermRule : public ParentRule
{
public:
    TermRule();
    ~TermRule() override = default;

    bool initialize(JackTokenizer* pTokenizer) override;
    void compile(VMWriter* vmWriter) override;

private:
    std::vector<CreateRuleFunc> mCreateRuleFuncs;
};

class SubroutineCallRule : public AlternationRule
{
public:
    SubroutineCallRule();
    ~SubroutineCallRule() override = default;

    void setRuleLevel(int ruleLevel) override;
};

class ExpressionListRule : public SequenceRule
{
public:
    ExpressionListRule();
    ~ExpressionListRule() override = default;

    void compile(VMWriter* vmWriter) override;
};

class OpRule : public AlternationRule
{
public:
    OpRule();
    ~OpRule() override = default;
};

class UnaryOpRule : public AlternationRule
{
public:
    UnaryOpRule();
    ~UnaryOpRule() override = default;
};

class KeywordConstantRule : public AlternationRule
{
public:
    KeywordConstantRule();
    ~KeywordConstantRule() override = default;
};
