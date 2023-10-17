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
struct CompileResult
{
    bool compileResult;
    bool tokenConsumed;
};

class Rule
{
public:
    Rule() {};
    virtual ~Rule() {};

public:
    /// <summary>
    /// Compiles the current rule.
    /// </summary>
    /// <param name="pTokenizer">A pointer to the JackTokenizer for tokens information.</param>
    /// <param name="advanceToken">An information if tokenizer should be advanced to the next token.</param>
    /// <returns>
    /// A tuple containing two values:
    ///   - First value is the compile result.
    ///   - Second value indicates if the token was consumed.
    /// </returns>
    virtual CompileResult compile(JackTokenizer* pTokenizer, bool advanceToken = true) final;

protected:
    virtual void beforeCompile(JackTokenizer* pTokenizer, bool advanceToken);
    virtual bool doCompile(JackTokenizer* pTokenizer);
    virtual CompileResult afterCompile(JackTokenizer* pTokenizer, bool compileResult);
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

protected:
    bool doCompile(JackTokenizer* pTokenizer) override;
};

class AlternationRule : public ParentRule
{
public:
    AlternationRule(RuleVector rules);
    virtual ~AlternationRule() {};

protected:
    bool doCompile(JackTokenizer* pTokenizer) override;
};

class ZeroOrMoreRule : public ParentRule
{
public:
    ZeroOrMoreRule(RuleVector rules);
    virtual ~ZeroOrMoreRule() {};

protected:
    bool doCompile(JackTokenizer* pTokenizer) override;
    CompileResult afterCompile(JackTokenizer* pTokenizer, bool compileResult) override;
};

class ZeroOrOneRule : public ParentRule
{
public:
    ZeroOrOneRule(RuleVector rules);
    virtual ~ZeroOrOneRule() {};

protected:
    bool doCompile(JackTokenizer* pTokenizer) override;
    CompileResult afterCompile(JackTokenizer* pTokenizer, bool compileResult);
};