#pragma once

#include "BaseRules.h"
#include "LexicalRules.h"
#include "..\SymbolTable.h"

class ClassRule;
class ClassVarDecRule;
class TypeRule;
class SubroutineDecRule;
class ParameterListRule;
class SubroutineBodyRule;
class VarDecRule;
class ClassNameRule;
class SubroutineNameRule;
class VarNameRule;


class ClassRule : public SequenceRule
{
public:
    ClassRule();
    ~ClassRule() override = default;

    void compile() override;

    SymbolTable& getSymbolTable();

private:
    SymbolTable mSymbolTable;
};

class ClassVarDecRule : public SequenceRule
{
public:
    ClassVarDecRule();
    ~ClassVarDecRule() override = default;

    bool initialize(JackTokenizer* pTokenizer) override;
    void compile() override;
};

class TypeRule : public AlternationRule
{
public:
    TypeRule();
    ~TypeRule() override = default;
};

class SubroutineDecRule : public SequenceRule
{
public:
    SubroutineDecRule();
    ~SubroutineDecRule() override = default;

    void compile() override;

    SymbolTable& getSymbolTable();

private:
    SymbolTable mSymbolTable;
};

class ParameterListRule : public ZeroOrOneRule
{
public:
    ParameterListRule();
    ~ParameterListRule() override = default;

    bool initialize(JackTokenizer* pTokenizer) override;
    void compile() override;
};

class SubroutineBodyRule : public SequenceRule
{
public:
    SubroutineBodyRule();
    ~SubroutineBodyRule() override = default;

    void compile() override;
};

class VarDecRule : public SequenceRule
{
public:
    VarDecRule();
    ~VarDecRule() override = default;

    void compile() override;
};

class ClassNameRule : public IdentifierRule
{
public:
    ClassNameRule() = default;
    ~ClassNameRule() override = default;
};

class SubroutineNameRule : public IdentifierRule
{
public:
    SubroutineNameRule() = default;
    ~SubroutineNameRule() override = default;
};

class VarNameRule : public IdentifierRule
{
public:
    VarNameRule() = default;
    ~VarNameRule() override = default;
};
