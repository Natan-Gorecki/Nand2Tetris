#include "BaseRules.h"
#include "ProgramStructureRules.h"
#include "RuleUtils.h"
#include "../JackCompilerError.h"

Symbol RuleUtils::findSymbol(Rule* rule, const std::string& name)
{
    auto& classTable = rule->getParentRecursive<ClassRule>()->getSymbolTable();
    auto& subroutineTable = rule->getParentRecursive<SubroutineDecRule>()->getSymbolTable();

    auto symbol = subroutineTable.getSymbol(name);
    if (symbol.kind == ESymbolKind::UNDEFINED)
    {
        symbol = classTable.getSymbol(name);
    }

    return symbol;
}