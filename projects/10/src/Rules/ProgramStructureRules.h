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

public:
    void compile() override;
};

class ClassVarDecRule : public SequenceRule
{
public:
    ClassVarDecRule();
    virtual ~ClassVarDecRule() {};

public:
    void compile() override;
};

class TypeRule : public AlternationRule
{
public:
    TypeRule();
    virtual ~TypeRule() {};
};

class SubroutineDecRule : public SequenceRule
{
public:
    SubroutineDecRule();
    virtual ~SubroutineDecRule() {};

public:
    void compile() override;
};

class ParameterListRule : public ZeroOrOneRule
{
public:
    ParameterListRule();
    virtual ~ParameterListRule() {};

public:
    void compile() override;
};

class SubroutineBodyRule : public SequenceRule
{
public:
    SubroutineBodyRule();
    virtual ~SubroutineBodyRule() {};

public:
    void compile() override;
};

class VarDecRule : public SequenceRule
{
public:
    VarDecRule();
    virtual ~VarDecRule() {};

public:
    void compile() override;
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
