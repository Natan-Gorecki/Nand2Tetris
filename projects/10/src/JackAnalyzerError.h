#pragma once
#include <stdexcept>
#include <string>

class JackAnalyzerError : public std::runtime_error
{
    using std::runtime_error::runtime_error;
};
