#pragma once

#include <functional>
#include <string>
#include <vector>
#include "../JackTokenizer.h"


class Rule;
class ParentRule;
class SequenceRule;
class AlternationRule;
class ZeroOrMoreRule;
class ZeroOrOneRule;


using RuleVector = std::vector<Rule*>;
using CreateRuleFunc = std::function<Rule* (void)>;

class Rule
{
public:
    Rule() {};
    virtual ~Rule() {};

public:
    virtual bool initialize(JackTokenizer* pTokenizer);
    virtual void compile();

public:
    virtual void setRuleLevel(int ruleLevel);
protected:
    void writeOutput(std::string text);
    void writeToken(std::string text);

protected:
    int mRuleLevel = 0;
};

class ParentRule : public Rule
{
public:
    ParentRule(RuleVector rules);
    virtual ~ParentRule();

public:
    void compile() override;

protected:
    std::vector<Rule*> mChildRules;
};

class SequenceRule : public ParentRule
{
public:
    SequenceRule(RuleVector rules);
    virtual ~SequenceRule() {};

public:
    bool initialize(JackTokenizer* pTokenizer) override;
};

class AlternationRule : public ParentRule
{
public:
    AlternationRule(RuleVector rules);
    virtual ~AlternationRule() {};

public:
    bool initialize(JackTokenizer* pTokenizer) override;
    void compile() override;

public:
    void setRuleLevel(int ruleLevel);

private:
    Rule* mCompileRule = nullptr;
};

class ZeroOrMoreRule : public ParentRule
{
public:
    ZeroOrMoreRule(CreateRuleFunc createRuleFunc);
    virtual ~ZeroOrMoreRule() {};

public:
    bool initialize(JackTokenizer* pTokenizer) override;

private:
    CreateRuleFunc onCreateRule;
};

class ZeroOrOneRule : public ParentRule
{
public:
    ZeroOrOneRule(CreateRuleFunc createRuleFunc);
    virtual ~ZeroOrOneRule() {};

public:
    bool initialize(JackTokenizer* pTokenizer) override;

private:
    CreateRuleFunc onCreateRule;
};