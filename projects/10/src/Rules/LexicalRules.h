#pragma once

#include <string>
#include "BaseRules.h"

class LexicalRule;
class KeywordRule;
class SymbolRule;
class IntegerConstantRule;
class StringConstantRule;
class IdentifierRule;


class LexicalRule : public Rule
{
public:
    LexicalRule() = default;
    ~LexicalRule() override = default;

    bool initialize(JackTokenizer* pTokenizer) final;
    void compile() final;

protected:
    virtual bool isFullfiled(JackTokenizer* pTokenizer) = 0;
    virtual std::string toString() = 0;
};

class KeywordRule : public LexicalRule
{
public:
    /// <summary>
    /// Returns information if passed string is a keyword.
    /// </summary>
    static bool isKeyword(std::string const& name);

    explicit KeywordRule(std::string const& keyword);
    ~KeywordRule() override = default;

protected:
    bool isFullfiled(JackTokenizer* pTokenizer) override;
    std::string toString() override;

private:
    std::string mKeyword;
};

class SymbolRule : public LexicalRule
{
public:
    /// <summary>
    /// Returns information if passed character is symbol.
    /// </summary>
    static bool isSymbol(char symbol);

    explicit SymbolRule(char symbol);
    ~SymbolRule() override = default;

protected:
    bool isFullfiled(JackTokenizer* pTokenizer) override;
    std::string toString() override;

private:
    std::string encodeXmlSymbol(char symbol) const;
    char mSymbol;
};

class IntegerConstantRule : public LexicalRule
{
public:
    IntegerConstantRule() = default;
    ~IntegerConstantRule() override = default;

protected:
    bool isFullfiled(JackTokenizer* pTokenizer) override;
    std::string toString() override;

private:
    int mIntVal = 0;
};

class StringConstantRule : public LexicalRule
{
public:
    StringConstantRule() = default;
    ~StringConstantRule() override = default;

protected:
    bool isFullfiled(JackTokenizer* pTokenizer) override;
    std::string toString() override;

private:
    std::string mStringVal;
};

class IdentifierRule : public LexicalRule
{
public:
    IdentifierRule() = default;
    ~IdentifierRule() override = default;

protected:
    bool isFullfiled(JackTokenizer* pTokenizer) override;
    std::string toString() override;

private:
    std::string mIdentifier;
};
