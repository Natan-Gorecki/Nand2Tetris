#include "KeywordTable.h"

std::set<std::string> keywords =
{
	"class",
	"constructor",
	"function",
	"method",
	"field",
	"static",
	"var",
	"int",
	"char",
	"boolean",
	"void",
	"true",
	"false",
	"null",
	"this",
	"let",
	"do",
	"if",
	"else",
	"while",
	"return"
};

/// <summary>
/// Returns information if passed string is a keyword.
/// </summary>
bool KeywordTable::isKeyword(std::string name)
{
	return keywords.count(name) != 0;
}
