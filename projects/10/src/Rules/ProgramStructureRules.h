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


class ClassRule : public ParentRule
{
public:
    ClassRule();
    ~ClassRule() {};
};

class ClassVarDecRule : public ParentRule
{
public:
    ClassVarDecRule();
    ~ClassVarDecRule() {};
};

class TypeRule : public ParentRule
{
public:
    TypeRule();
    ~TypeRule() {};
};

class SubroutineDecRule : public ParentRule
{
public:
    SubroutineDecRule();
    ~SubroutineDecRule() {};
};

class ParameterListRule : public ParentRule
{
public:
    ParameterListRule();
    ~ParameterListRule() {};
};

class SubroutineBodyRule : public ParentRule
{
public:
    SubroutineBodyRule();
    ~SubroutineBodyRule() {};
};

class VarDecRule : public ParentRule
{
public:
    VarDecRule();
    ~VarDecRule() {};
};

class ClassNameRule : public IdentifierRule
{
public:
    ClassNameRule();
    ~ClassNameRule() {};
};

class SubroutineNameRule : public IdentifierRule
{
public:
    SubroutineNameRule();
    ~SubroutineNameRule() {};
};

class VarNameRule : public IdentifierRule
{
public:
    VarNameRule();
    ~VarNameRule() {};
};
