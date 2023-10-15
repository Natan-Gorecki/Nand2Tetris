#pragma once

#include <functional>
#include <vector>
#include "../JackTokenizer.h"


class Rule;
class ParentRule;
class SequenceRule;
class AlternationRule;
class ZeroOrMoreRule;
class ZeroOrOneRule;


using RuleVector = std::vector<Rule*>;
using StreamWriteFunc = std::function<void(std::string)>;


class Rule
{
public:
    Rule() {};
    virtual ~Rule() {};

public:
    virtual bool compile(JackTokenizer* pTokenizer) {};

public:
    static void setOutputFunc(StreamWriteFunc pOutputStream);
    static void setTokensFunc(StreamWriteFunc pTokenStream);

protected:
    static StreamWriteFunc onWriteOutput;
    static StreamWriteFunc onWriteToken;
};

class ParentRule : public Rule
{
public:
    ParentRule(RuleVector rules);
    virtual ~ParentRule();

protected:
    std::vector<Rule*> mChildRules;
};

class SequenceRule : public ParentRule
{
public:
    SequenceRule(RuleVector rules);
    virtual ~SequenceRule() {};

public:
    bool compile(JackTokenizer* pTokenizer) override;
};

class AlternationRule : public ParentRule
{
public:
    AlternationRule(RuleVector rules);
    virtual ~AlternationRule() {};

public:
    bool compile(JackTokenizer* pTokenizer) override;
};

class ZeroOrMoreRule : public ParentRule
{
public:
    ZeroOrMoreRule(RuleVector rules);
    virtual ~ZeroOrMoreRule() {};

public:
    bool compile(JackTokenizer* pTokenizer) override;
};

class ZeroOrOneRule : public ParentRule
{
public:
    ZeroOrOneRule(RuleVector rules);
    virtual ~ZeroOrOneRule() {};

public:
    bool compile(JackTokenizer* pTokenizer) override;
};