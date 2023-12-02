#pragma once

#include <string>
#include "ESegment.h"
#include "ESymbolKind.h"

struct Symbol
{
    std::string name;
    ESymbolKind kind;
    std::string type;
    int index;

    ESegment getSegment()
    {
        switch (kind)
        {
        case ESymbolKind::STATIC:
            return ESegment::STATIC;
        case ESymbolKind::FIELD:
            return ESegment::THIS;
        case ESymbolKind::ARG:
            return ESegment::ARGUMENT;
        case ESymbolKind::VAR:
            return ESegment::LOCAL;
        case ESymbolKind::UNDEFINED:
        default:
            return ESegment::UNDEFINED;
        }
    }
};
