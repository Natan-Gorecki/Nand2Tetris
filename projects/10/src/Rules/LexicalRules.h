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
    LexicalRule() {};
    virtual ~LexicalRule() {};

public:
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
    static bool isKeyword(std::string name);

public:
    KeywordRule(std::string keyword);
    virtual ~KeywordRule() {};

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

public:
    SymbolRule(char symbol);
    virtual ~SymbolRule() {};

protected:
    bool isFullfiled(JackTokenizer* pTokenizer) override;
    std::string toString() override;

private:
    std::string encodeXmlSymbol(char symbol);
    char mSymbol;
};

class IntegerConstantRule : public LexicalRule
{
public:
    IntegerConstantRule() {};
    virtual ~IntegerConstantRule() {};

protected:
    bool isFullfiled(JackTokenizer* pTokenizer) override;
    std::string toString() override;

private:
    int mIntVal;
};

class StringConstantRule : public LexicalRule
{
public:
    StringConstantRule() {};
    virtual ~StringConstantRule() {};

protected:
    bool isFullfiled(JackTokenizer* pTokenizer) override;
    std::string toString() override;

private:
    std::string mStringVal;
};

class IdentifierRule : public LexicalRule
{
public:
    IdentifierRule() {};
    virtual ~IdentifierRule() {};

protected:
    bool isFullfiled(JackTokenizer* pTokenizer) override;
    std::string toString() override;

private:
    std::string mIdentifier;
};
