#pragma once

#include "BaseRules.h"


class StatementsRule;
class StatementRule;
class LetStatementRule;
class IfStatementRule;
class WhileStatementRule;
class DoStatementRule;
class ReturnStatementRule;


class StatementsRule : public SequenceRule
{
public:
    StatementsRule();
    ~StatementsRule() override = default;

    void compile() override;
};

class StatementRule : public AlternationRule
{
public:
    StatementRule();
    ~StatementRule() override = default;
};

class LetStatementRule : public SequenceRule
{
public:
    LetStatementRule();
    ~LetStatementRule() override = default;

    void compile() override;
};

class IfStatementRule : public SequenceRule
{
public:
    IfStatementRule();
    ~IfStatementRule() override = default;

    void compile() override;
};

class WhileStatementRule : public SequenceRule
{
public:
    WhileStatementRule();
    ~WhileStatementRule() override = default;

    void compile() override;
};

class DoStatementRule : public SequenceRule
{
public:
    DoStatementRule();
    ~DoStatementRule() override = default;

    void compile() override;
};

class ReturnStatementRule : public SequenceRule
{
public:
    ReturnStatementRule();
    ~ReturnStatementRule() override = default;

    void compile() override;
};
