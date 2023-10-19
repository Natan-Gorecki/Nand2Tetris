#pragma once

#include "BaseRules.h"
#include "LexicalRules.h"


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
    virtual ~ClassRule() {};

protected:
    bool doCompile(JackTokenizer* pTokenizer) override;
};

class ClassVarDecRule : public SequenceRule
{
public:
    ClassVarDecRule();
    virtual ~ClassVarDecRule() {};

protected:
    bool doCompile(JackTokenizer* pTokenizer) override;
};

class TypeRule : public AlternationRule
{
public:
    TypeRule();
    virtual ~TypeRule() {};

protected:
    bool doCompile(JackTokenizer* pTokenizer) override;
};

class SubroutineDecRule : public SequenceRule
{
public:
    SubroutineDecRule();
    virtual ~SubroutineDecRule() {};

protected:
    bool doCompile(JackTokenizer* pTokenizer) override;
};

class ParameterListRule : public ZeroOrOneRule
{
public:
    ParameterListRule();
    virtual ~ParameterListRule() {};

protected:
    bool doCompile(JackTokenizer* pTokenizer) override;
};

class SubroutineBodyRule : public SequenceRule
{
public:
    SubroutineBodyRule();
    virtual ~SubroutineBodyRule() {};

protected:
    bool doCompile(JackTokenizer* pTokenizer) override;
};

class VarDecRule : public SequenceRule
{
public:
    VarDecRule();
    virtual ~VarDecRule() {};

protected:
    bool doCompile(JackTokenizer* pTokenizer) override;
};

class ClassNameRule : public IdentifierRule
{
public:
    ClassNameRule() {};
    virtual ~ClassNameRule() {};
};

class SubroutineNameRule : public IdentifierRule
{
public:
    SubroutineNameRule() {};
    virtual ~SubroutineNameRule() {};
};

class VarNameRule : public IdentifierRule
{
public:
    VarNameRule() {};
    virtual ~VarNameRule() {};
};
