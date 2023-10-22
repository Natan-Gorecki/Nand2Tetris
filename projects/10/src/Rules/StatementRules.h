#pragma once

#include "BaseRules.h"


class StatementsRule;
class StatementRule;
class LetStatementRule;
class IfStatementRule;
class WhileStatementRule;
class DoStatementRule;
class ReturnStatementRule;


class StatementsRule : public ZeroOrMoreRule
{
public:
    StatementsRule();
    virtual ~StatementsRule() {};

public:
    void compile() override;
};

class StatementRule : public AlternationRule
{
public:
    StatementRule();
    virtual ~StatementRule() {};
};

class LetStatementRule : public SequenceRule
{
public:
    LetStatementRule();
    virtual ~LetStatementRule() {};

public:
    void compile() override;
};

class IfStatementRule : public SequenceRule
{
public:
    IfStatementRule();
    virtual ~IfStatementRule() {};

public:
    void compile() override;
};

class WhileStatementRule : public SequenceRule
{
public:
    WhileStatementRule();
    virtual ~WhileStatementRule() {};

public:
    void compile() override;
};

class DoStatementRule : public SequenceRule
{
public:
    DoStatementRule();
    virtual ~DoStatementRule() {};

public:
    void compile() override;
};

class ReturnStatementRule : public SequenceRule
{
public:
    ReturnStatementRule();
    virtual ~ReturnStatementRule() {};

public:
    void compile() override;
};
