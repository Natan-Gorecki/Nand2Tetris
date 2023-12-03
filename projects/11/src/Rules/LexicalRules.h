#pragma once

#include <string>
#include "BaseRules.h"

#define CLASS_NAME_OVERRIDE(className) std::string getClassName() override {\
    return className;\
}

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
    void writeXmlSyntax(std::ofstream* stream, int tabs) final;
    void writeXmlTokens(std::ofstream* stream) final;

    virtual std::string toString() = 0;

protected:
    virtual bool isFullfiled(JackTokenizer* pTokenizer) = 0;
    virtual std::string getClassName() = 0;

private:
    std::string encodeXml(const std::string& text) const;
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

    std::string toString() override;

protected:
    bool isFullfiled(JackTokenizer* pTokenizer) override;
    CLASS_NAME_OVERRIDE("keyword");

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

    std::string toString() override;
    char getValue() const;

protected:
    bool isFullfiled(JackTokenizer* pTokenizer) override;
    CLASS_NAME_OVERRIDE("symbol");

private:
    char mSymbol;
};

class IntegerConstantRule : public LexicalRule
{
public:
    IntegerConstantRule() = default;
    ~IntegerConstantRule() override = default;

    std::string toString() override;
    int getValue() const;

protected:
    bool isFullfiled(JackTokenizer* pTokenizer) override;
    CLASS_NAME_OVERRIDE("integerConstant");

private:
    int mIntVal = 0;
};

class StringConstantRule : public LexicalRule
{
public:
    StringConstantRule() = default;
    ~StringConstantRule() override = default;

    std::string toString() override;

protected:
    bool isFullfiled(JackTokenizer* pTokenizer) override;
    CLASS_NAME_OVERRIDE("stringConstant");

private:
    std::string mStringVal;
};

class IdentifierRule : public LexicalRule
{
public:
    IdentifierRule() = default;
    ~IdentifierRule() override = default;

    std::string toString() override;

protected:
    bool isFullfiled(JackTokenizer* pTokenizer) override;
    CLASS_NAME_OVERRIDE("identifier");

private:
    std::string mIdentifier;
};
