#pragma once

#include<vector>


class Rule;
class ParentRule;
class SequenceRule;
class AlternationRule;
class ZeroOrMoreRule;
class ZeroOrOneRule;


typedef std::vector<Rule*> RuleVector;


class Rule
{
public:
    Rule();
    ~Rule() {};
};

class ParentRule : public Rule
{
public:
    ParentRule(RuleVector rules);
    ~ParentRule() {};

protected:
    std::vector<Rule*> mChildRules;
};

class SequenceRule : public ParentRule
{
public:
    SequenceRule(RuleVector rules);
    ~SequenceRule() {};
};

class AlternationRule : public ParentRule
{
public:
    AlternationRule(RuleVector rules);
    ~AlternationRule() {};
};

class ZeroOrMoreRule : public Rule
{
public:
    ZeroOrMoreRule(RuleVector rules);
    ~ZeroOrMoreRule() {};
};

class ZeroOrOneRule : public Rule
{
public:
    ZeroOrOneRule(RuleVector rules);
    ~ZeroOrOneRule() {};
};