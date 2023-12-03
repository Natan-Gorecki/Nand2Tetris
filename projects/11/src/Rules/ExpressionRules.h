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
    void writeXmlSyntax(std::ofstream* stream, int tabs) override;
};

class TermRule : public Rule
{
public:
    TermRule();
    ~TermRule() override = default;

    bool initialize(JackTokenizer* pTokenizer) override;
    void compile(VMWriter* vmWriter) override;
    void writeXmlSyntax(std::ofstream* stream, int tabs) override;

private:
    std::vector<CreateRuleFunc> mCreateRuleFuncs;
};

class SubroutineCallRule : public AlternationRule
{
public:
    SubroutineCallRule();
    ~SubroutineCallRule() override = default;

    void compile(VMWriter* vmWriter) override;
    void writeXmlSyntax(std::ofstream* stream, int tabs) override;
};

class ExpressionListRule : public SequenceRule
{
public:
    ExpressionListRule();
    ~ExpressionListRule() override = default;

    void writeXmlSyntax(std::ofstream* stream, int tabs) override;

    int getExpressionCount();
};

class OpRule : public AlternationRule
{
public:
    OpRule();
    ~OpRule() override = default;

    void compile(VMWriter* vmWriter) override;
};

class UnaryOpRule : public AlternationRule
{
public:
    UnaryOpRule();
    ~UnaryOpRule() override = default;

    void compile(VMWriter* vmWriter) override;
};

class KeywordConstantRule : public AlternationRule
{
public:
    KeywordConstantRule();
    ~KeywordConstantRule() override = default;

    void compile(VMWriter* vmWriter) override;
};
