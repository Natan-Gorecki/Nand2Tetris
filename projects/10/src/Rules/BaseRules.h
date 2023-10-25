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


using RuleVector = std::vector<std::shared_ptr<Rule>>;
using CreateRuleFunc = std::function<std::shared_ptr<Rule>(void)>;

class Rule
{
public:
    Rule() = default;
    virtual ~Rule() = default;

    virtual bool initialize(JackTokenizer* pTokenizer);
    virtual void compile();

    int getRuleLevel() const;
    virtual void setRuleLevel(int ruleLevel);

protected:
    void writeOutput(std::string const& text) const;
    void writeToken(std::string const& text) const;

private:
    int mRuleLevel = 0;
};

class ParentRule : public Rule
{
public:
    explicit ParentRule(RuleVector const& rules);
    ~ParentRule() override = default;

    void compile() override;

protected:
    RuleVector& getChildRules();

private:
    RuleVector mChildRules;
};

class SequenceRule : public ParentRule
{
public:
    explicit SequenceRule(RuleVector const& rules);
    ~SequenceRule() override = default;

    bool initialize(JackTokenizer* pTokenizer) override;
};

class AlternationRule : public ParentRule
{
public:
    explicit AlternationRule(RuleVector const& rules);
    ~AlternationRule() override = default;

    bool initialize(JackTokenizer* pTokenizer) override;
    void compile() override;

    void setRuleLevel(int ruleLevel) override;

private:
    std::shared_ptr<Rule> mCompileRule = nullptr;
};

class ZeroOrMoreRule : public ParentRule
{
public:
    explicit ZeroOrMoreRule(CreateRuleFunc const& createRuleFunc);
    ~ZeroOrMoreRule() override = default;

    bool initialize(JackTokenizer* pTokenizer) override;

private:
    CreateRuleFunc onCreateRule;
};

class ZeroOrOneRule : public ParentRule
{
public:
    explicit ZeroOrOneRule(CreateRuleFunc const& createRuleFunc);
    ~ZeroOrOneRule() override = default;

    bool initialize(JackTokenizer* pTokenizer) override;

private:
    CreateRuleFunc onCreateRule;
};