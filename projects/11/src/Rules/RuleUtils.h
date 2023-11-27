#pragma once

#include "BaseRules.h"

class RuleUtils
{
public:
    template <typename TRule>
    static TRule* getParentRule(Rule* pRule)
    {
        Rule* pParentRule = pRule->getParentRule();

        while (pParentRule != nullptr)
        {
            if (auto pCastedRule = dynamic_cast<TRule*>(pParentRule))
            {
                return pCastedRule;
            }

            pParentRule = pParentRule->getParentRule();
        }

        return nullptr;
    }

    template <typename TRule>
    static TRule* getChildRule(ParentRule* pRule)
    {
        for (const auto& pChildRule : pRule.getChildRules())
        {
            if (auto pCastedRule = dynamic_cast<TRule*>(pChildRule))
            {
                return pCastedRule;
            }

            ParentRule* pParentRule ;

            if (auto pParentRule = dynamic_cast<ParentRule*>(pChildRule))
            {
                if (auto pCastedChild = GetChildRule(pParentRule))
                {
                    return pCastedChild;
                }
            }
        }

        return nullptr;
    }
};