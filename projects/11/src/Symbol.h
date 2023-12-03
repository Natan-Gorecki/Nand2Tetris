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

    ESegment getSegment() const
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
        default:
            return ESegment::UNDEFINED;
        }
    }
};
