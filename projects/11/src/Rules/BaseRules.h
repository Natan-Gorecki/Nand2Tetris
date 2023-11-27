#pragma once

#include <functional>
#include <string>
#include <vector>
#include "../JackTokenizer.h"
#include "../VMWriter.h"

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
    virtual void compile(VMWriter* vmWriter);

    Rule* getParentRule();
    void setParentRule(Rule* pRule);

    int getRuleLevel() const;
    virtual void setRuleLevel(int ruleLevel);

    template <typename TRule> TRule* cast()
    {
        return dynamic_cast<TRule*>(this);
    }

protected:
    void writeOutput(std::string const& text) const;
    void writeToken(std::string const& text) const;

private:
    Rule* mParentRule = nullptr;
    int mRuleLevel = 0;
};

class ParentRule : public Rule
{
public:
    explicit ParentRule(RuleVector const& rules);
    ~ParentRule() override = default;

    bool initialize(JackTokenizer* pTokenizer) override;
    void compile(VMWriter* vmWriter) override;

    RuleVector& getChildRules();
    template <typename TRule> TRule* getChildRule(int index)
    {
        return dynamic_cast<TRule*>(mChildRules[index].get());
    }

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
    void compile(VMWriter* vmWriter) override;

    void setRuleLevel(int ruleLevel) override;
    Rule* getPassedRule();

private:
    std::shared_ptr<Rule> mPassedRule = nullptr;
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