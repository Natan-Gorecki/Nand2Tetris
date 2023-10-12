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
    ~StatementsRule() {};
};

class StatementRule : public ParentRule
{
public:
    StatementRule();
    ~StatementRule() {};
};

class LetStatementRule : public ParentRule
{
public:
    LetStatementRule();
    ~LetStatementRule() {};
};

class IfStatementRule : public ParentRule
{
public:
    IfStatementRule();
    ~IfStatementRule() {};
};

class WhileStatementRule : public ParentRule
{
public:
    WhileStatementRule();
    ~WhileStatementRule() {};
};

class DoStatementRule : public ParentRule
{
public:
    DoStatementRule();
    ~DoStatementRule() {};
};

class ReturnStatementRule : public ParentRule
{
public:
    ReturnStatementRule();
    ~ReturnStatementRule() {};
};
