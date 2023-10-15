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
    bool compile(JackTokenizer* pTokenizer) override final;

protected:
    virtual bool lexicalCompile(JackTokenizer* pTokenizer) = 0;
    virtual std::string toString(JackTokenizer* pTokenizer) = 0;
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
    bool lexicalCompile(JackTokenizer* pTokenizer) override;
    std::string toString(JackTokenizer* pTokenizer) override;

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
    bool lexicalCompile(JackTokenizer* pTokenizer) override;
    std::string toString(JackTokenizer* pTokenizer) override;

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
    bool lexicalCompile(JackTokenizer* pTokenizer) override;
    std::string toString(JackTokenizer* pTokenizer) override;
};

class StringConstantRule : public LexicalRule
{
public:
    StringConstantRule() {};
    virtual ~StringConstantRule() {};

protected:
    bool lexicalCompile(JackTokenizer* pTokenizer) override;
    std::string toString(JackTokenizer* pTokenizer) override;
};

class IdentifierRule : public LexicalRule
{
public:
    IdentifierRule() {};
    virtual ~IdentifierRule() {};

protected:
    bool lexicalCompile(JackTokenizer* pTokenizer) override;
    std::string toString(JackTokenizer* pTokenizer) override;
};
