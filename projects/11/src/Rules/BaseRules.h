#pragma once

#include <functional>
#include <string>
#include <vector>
#include "../JackTokenizer.h"
#include "../VMWriter.h"

class Rule;
class SequenceRule;
class AlternationRule;
class ZeroOrMoreRule;
class ZeroOrOneRule;


using RuleVector = std::vector<std::shared_ptr<Rule>>;
using CreateRuleFunc = std::function<std::shared_ptr<Rule>(void)>;

#define XML_SYNTAX(text) *stream << std::string(tabs * 2, ' ') << text << "\n";

class Rule
{
public:
    Rule() = default;
    Rule(RuleVector const& rules);
    virtual ~Rule() = default;

    virtual bool initialize(JackTokenizer* pTokenizer);
    virtual void compile(VMWriter* vmWriter);
    virtual void writeXmlSyntax(std::ofstream* stream, int tabs);
    virtual void writeXmlTokens(std::ofstream* stream);

    Rule* getParent();
    void setParent(Rule* pRule);
    template <typename TRule> TRule* getParentRecursive()
    {
        Rule* parentRule = getParent();

        while (parentRule != nullptr)
        {
            if (auto castedRule = dynamic_cast<TRule*>(parentRule))
            {
                return castedRule;
            }

            parentRule = parentRule->getParent();
        }

        return nullptr;
    }

    Rule* getChild(int index);
    RuleVector& getChildRules();

    template <typename TRule> TRule* cast()
    {
        return dynamic_cast<TRule*>(this);
    }

private:
    Rule* mParentRule = nullptr;
    RuleVector mChildRules;
};

class SequenceRule : public Rule
{
public:
    explicit SequenceRule(RuleVector const& rules);
    ~SequenceRule() override = default;

    bool initialize(JackTokenizer* pTokenizer) override;
    void writeXmlSyntax(std::ofstream* stream, int tabs) override;
};

class AlternationRule : public Rule
{
public:
    explicit AlternationRule(RuleVector const& rules);
    ~AlternationRule() override = default;

    bool initialize(JackTokenizer* pTokenizer) override;
    void compile(VMWriter* vmWriter) override;
    void writeXmlSyntax(std::ofstream* stream, int tabs) override;

    Rule* getTrueRule();

private:
    std::shared_ptr<Rule> mPassedRule = nullptr;
};

class ZeroOrMoreRule : public Rule
{
public:
    explicit ZeroOrMoreRule(CreateRuleFunc const& createRuleFunc);
    ~ZeroOrMoreRule() override = default;

    bool initialize(JackTokenizer* pTokenizer) override;
    void writeXmlSyntax(std::ofstream* stream, int tabs) override;

private:
    CreateRuleFunc onCreateRule;
};

class ZeroOrOneRule : public Rule
{
public:
    explicit ZeroOrOneRule(CreateRuleFunc const& createRuleFunc);
    ~ZeroOrOneRule() override = default;

    bool initialize(JackTokenizer* pTokenizer) override;
    void writeXmlSyntax(std::ofstream* stream, int tabs) override;

private:
    CreateRuleFunc onCreateRule;
};