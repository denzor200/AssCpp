#include "stdafx.h"
#include "Parentheses.h"

// Изменять содержимое StrInout разрешается только если НЕ установлен соответствующий флаг (P.ForbidModifyInoutString)
template <typename IS_BEGIN_FUNC, typename IS_END_FUNC>
static
size_t
ExtractString(
	std::string& StrInout,
	std::string* StrOut,
	size_t Pos,
	IS_BEGIN_FUNC IsBegin, IS_END_FUNC IsEnd,
	Parentheses::Params P)
{
	std::string::iterator begin, end;
	size_t begin_pos = Parentheses::NOT_FOUND;
	bool has_Parentheses = false;
	std::stack<char> PStack;

	size_t i = Pos;
	for (auto it = StrInout.begin() + Pos; it != StrInout.end(); ++it, ++i)
	{
		if (IsBegin(*it) && i <= P.Limits.begin)
		{
			// we need only first
			if (PStack.size() == 0)
			{
				if (Parentheses::NOT_FOUND == i)
					return Parentheses::NOT_FOUND;	// paranoia??
				begin = it;
				begin_pos = i; // store for return value..
			}
			PStack.push(*it);
		}
		else if (PStack.size() != 0 && (IsEnd(PStack.top(), *it) && i <= P.Limits.end))
		{
			if (PStack.size() == 1)
			{
				end = it;
				has_Parentheses = true;
				break;
			}

			PStack.pop();
		}
	}

	if (has_Parentheses)
	{
		assert(Parentheses::NOT_FOUND != begin_pos);
		if (StrOut)
			*StrOut = boost::trim_copy(Utils::CopyToNewString(begin + 1, end));
		if (!P.ForbidModifyInoutString)
		{
			StrInout.erase(begin, end + 1);
			if (P.TrimInoutString)
				boost::trim_left(StrInout);
		}
		return begin_pos;
	}
	return Parentheses::NOT_FOUND;
}

/******************************
* Predicates..
*******************************/
#define SQUARE_BEGIN_PRED ([](char c) {return c == '['; })
#define SQUARE_END_PRED ([](char,char c) {return c == ']'; })

#define ROUND_BEGIN_PRED ([](char c) {return c == '('; })
#define ROUND_END_PRED ([](char,char c) {return c == ')'; })

#define BRACES_BEGIN_PRED ([](char c) {return c == '{'; })
#define BRACES_END_PRED ([](char,char c) {return c == '}'; })

/******************************
* ExtractStringFrom__(std::string& StrInout, std::string& StrOut, Params P)
*******************************/
size_t Parentheses::ExtractStringFromSquare(std::string& StrInout, std::string& StrOut, size_t Pos, Params P)
{
	return ExtractString(StrInout, &StrOut, Pos, SQUARE_BEGIN_PRED, SQUARE_END_PRED, P);
}

size_t Parentheses::ExtractStringFromRound(std::string& StrInout, std::string& StrOut, size_t Pos, Params P)
{
	return ExtractString(StrInout, &StrOut, Pos, ROUND_BEGIN_PRED, ROUND_END_PRED, P);
}

size_t Parentheses::ExtractStringFromBraces(std::string& StrInout, std::string& StrOut, size_t Pos, Params P)
{
	return ExtractString(StrInout, &StrOut, Pos, BRACES_BEGIN_PRED, BRACES_END_PRED, P);
}



/******************************
* ExtractStringFrom__(std::string& StrInout, Params P)
*******************************/
size_t Parentheses::ExtractStringFromSquare(std::string& StrInout, size_t Pos, Params P)
{
	return ExtractString(StrInout, NULL, Pos, SQUARE_BEGIN_PRED, SQUARE_END_PRED, P);
}

size_t Parentheses::ExtractStringFromRound(std::string& StrInout, size_t Pos, Params P)
{
	return ExtractString(StrInout, NULL, Pos, ROUND_BEGIN_PRED, ROUND_END_PRED, P);
}

size_t Parentheses::ExtractStringFromBraces(std::string& StrInout, size_t Pos, Params P)
{
	return ExtractString(StrInout, NULL, Pos, BRACES_BEGIN_PRED, BRACES_END_PRED, P);
}


/******************************
* Predicates..
*******************************/
static bool AnyBeginPred(char c)
{
	return c == '{' || c == '(' || c == '[';
};
static bool AnyEndPred(char b, char c)
{
	switch (b)
	{
	case '{':
		return c == '}';
	case '(':
		return c == ')';
	case '[':
		return c == ']';
	}
	return false;
};

/******************************
* ExtractStringFromAny(std::string& StrInout, std::string& StrOut, Params P)
*******************************/
size_t Parentheses::ExtractStringFromAny(std::string& StrInout, std::string& StrOut, size_t Pos, Params P)
{
	return ExtractString(StrInout, &StrOut, Pos, AnyBeginPred, AnyEndPred, P);
}

/******************************
* ExtractStringFromAny(std::string& StrInout, Params P)
*******************************/
size_t Parentheses::ExtractStringFromAny(std::string& StrInout, size_t Pos, Params P)
{
	return ExtractString(StrInout, NULL, Pos, AnyBeginPred, AnyEndPred, P);
}



