#pragma once

#include "SubString.h"

namespace Parentheses
{
	struct Limits
	{
		static const size_t NoLimit = ((size_t)-1);

		size_t begin = NoLimit;
		size_t end = NoLimit;

		Limits() = default;
	};

	struct Params
	{
		Limits Limits = Parentheses::Limits();
		bool TrimInoutString = false;
		bool ForbidModifyInoutString = false;

		Params() = default;
	};

	static const size_t NOT_FOUND = ((size_t)-1);

	size_t ExtractStringFromSquare(std::string& StrInout, std::string& StrOut, size_t Pos = 0, Params P = Params());
	size_t ExtractStringFromRound(std::string& StrInout, std::string& StrOut, size_t Pos = 0, Params P = Params());
	size_t ExtractStringFromBraces(std::string& StrInout, std::string& StrOut, size_t Pos = 0, Params P = Params());
	size_t ExtractStringFromAny(std::string& StrInout, std::string& StrOut, size_t Pos = 0, Params P = Params());

	size_t ExtractStringFromSquare(std::string& StrInout, size_t Pos = 0, Params P = Params());
	size_t ExtractStringFromRound(std::string& StrInout, size_t Pos = 0, Params P = Params());
	size_t ExtractStringFromBraces(std::string& StrInout, size_t Pos = 0, Params P = Params());
	size_t ExtractStringFromAny(std::string& StrInout, size_t Pos = 0, Params P = Params());


	size_t ConstantExtractStringFromSquare(const std::string& StrIn, std::string& StrOut, size_t Pos = 0, Params P = Params());
	size_t ConstantExtractStringFromRound(const std::string& StrIn, std::string& StrOut, size_t Pos = 0, Params P = Params());
	size_t ConstantExtractStringFromBraces(const std::string& StrIn, std::string& StrOut, size_t Pos = 0, Params P = Params());
	size_t ConstantExtractStringFromAny(const std::string& StrIn, std::string& StrOut, size_t Pos = 0, Params P = Params());

	size_t ConstantExtractStringFromSquare(const std::string& StrIn, SUBSTRING& StrOut, size_t Pos = 0, Params P = Params());
	size_t ConstantExtractStringFromRound(const std::string& StrIn, SUBSTRING& StrOut, size_t Pos = 0, Params P = Params());
	size_t ConstantExtractStringFromBraces(const std::string& StrIn, SUBSTRING& StrOut, size_t Pos = 0, Params P = Params());
	size_t ConstantExtractStringFromAny(const std::string& StrIn, SUBSTRING& StrOut, size_t Pos = 0, Params P = Params());

};


inline size_t Parentheses::ConstantExtractStringFromSquare(const std::string& StrIn, std::string& StrOut, size_t Pos, Params P)
{
	P.ForbidModifyInoutString = true;
	return ExtractStringFromSquare(const_cast<std::string&>(StrIn), StrOut, Pos, P);
}

inline size_t Parentheses::ConstantExtractStringFromRound(const std::string& StrIn, std::string& StrOut, size_t Pos, Params P)
{
	P.ForbidModifyInoutString = true;
	return ExtractStringFromRound(const_cast<std::string&>(StrIn), StrOut, Pos, P);
}

inline size_t Parentheses::ConstantExtractStringFromBraces(const std::string& StrIn, std::string& StrOut, size_t Pos, Params P)
{
	P.ForbidModifyInoutString = true;
	return ExtractStringFromBraces(const_cast<std::string&>(StrIn), StrOut, Pos, P);
}

inline size_t Parentheses::ConstantExtractStringFromAny(const std::string& StrIn, std::string& StrOut, size_t Pos, Params P)
{
	P.ForbidModifyInoutString = true;
	return ExtractStringFromBraces(const_cast<std::string&>(StrIn), StrOut, Pos, P);
}

