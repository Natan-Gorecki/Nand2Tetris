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

class ClassVarDecRule : public ParentRule
{
public:
    ClassVarDecRule();
    virtual ~ClassVarDecRule() {};

protected:
    bool doCompile(JackTokenizer* pTokenizer) override;
};

class TypeRule : public ParentRule
{
public:
    TypeRule();
    virtual ~TypeRule() {};
};

class SubroutineDecRule : public ParentRule
{
public:
    SubroutineDecRule();
    virtual ~SubroutineDecRule() {};

protected:
    bool doCompile(JackTokenizer* pTokenizer) override;
};

class ParameterListRule : public ParentRule
{
public:
    ParameterListRule();
    virtual ~ParameterListRule() {};
};

class SubroutineBodyRule : public ParentRule
{
public:
    SubroutineBodyRule();
    virtual ~SubroutineBodyRule() {};
};

class VarDecRule : public ParentRule
{
public:
    VarDecRule();
    virtual ~VarDecRule() {};
};

class ClassNameRule : public IdentifierRule
{
public:
    ClassNameRule();
    virtual ~ClassNameRule() {};
};

class SubroutineNameRule : public IdentifierRule
{
public:
    SubroutineNameRule();
    virtual ~SubroutineNameRule() {};
};

class VarNameRule : public IdentifierRule
{
public:
    VarNameRule();
    virtual ~VarNameRule() {};
};
