#pragma once

#include "../Symbol.h"

class RuleUtils
{
public:
    static Symbol findSymbol(Rule* rule, std::string name);
};