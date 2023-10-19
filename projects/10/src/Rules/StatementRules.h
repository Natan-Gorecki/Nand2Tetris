#pragma once

#include "BaseRules.h"


class StatementsRule;
class StatementRule;
class LetStatementRule;
class IfStatementRule;
class WhileStatementRule;
class DoStatementRule;
class ReturnStatementRule;


class StatementsRule : public ParentRule
{
public:
    StatementsRule();
    virtual ~StatementsRule() {};

protected:
    bool doCompile(JackTokenizer* pTokenizer) override;
};

class StatementRule : public ParentRule
{
public:
    StatementRule();
    virtual ~StatementRule() {};
};

class LetStatementRule : public ParentRule
{
public:
    LetStatementRule();
    virtual ~LetStatementRule() {};
};

class IfStatementRule : public ParentRule
{
public:
    IfStatementRule();
    virtual ~IfStatementRule() {};
};

class WhileStatementRule : public ParentRule
{
public:
    WhileStatementRule();
    virtual ~WhileStatementRule() {};
};

class DoStatementRule : public ParentRule
{
public:
    DoStatementRule();
    virtual ~DoStatementRule() {};
};

class ReturnStatementRule : public ParentRule
{
public:
    ReturnStatementRule();
    virtual ~ReturnStatementRule() {};
};
