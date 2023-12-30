#pragma once

enum class ESegment
{
    UNDEFINED = 0,
    CONSTANT = 1,
    ARGUMENT = 2,
    LOCAL = 3,
    STATIC = 4,
    THIS = 5,
    THAT = 6,
    POINTER = 7,
    TEMP = 8
};
