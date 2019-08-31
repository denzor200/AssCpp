#include "stdafx.h"
#include "Parentheses.h"


// Изменять содержимое StrInout разрешается только если НЕ установлен соответствующий флаг (P.ForbidModifyInoutString)
template <typename IS_BEGIN_FUNC, typename IS_END_FUNC>
static
size_t
ExtractStringInternal(
	std::string& StrInout,
	void* StrOut,
	int StrOutType,
	size_t Pos,
	IS_BEGIN_FUNC IsBegin, IS_END_FUNC IsEnd,
	Parentheses::Params P)
{
	std::string::iterator begin, end;
	size_t begin_pos = Parentheses::NOT_FOUND;
	size_t end_pos = Parentheses::NOT_FOUND;
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
				begin_pos = i;
			}
			PStack.push(*it);
		}
		else if (PStack.size() != 0 && (IsEnd(PStack.top(), *it) && i <= P.Limits.end))
		{
			if (PStack.size() == 1)
			{
				end = it;
				end_pos = i;
				has_Parentheses = true;
				break;
			}

			PStack.pop();
		}
	}

	if (has_Parentheses)
	{
		assert(Parentheses::NOT_FOUND != begin_pos);
		// assert(Parentheses::NOT_FOUND != end_pos);
		switch (StrOutType)
		{
		case 0:
			assert(StrOut);
			SUBSTRING_Init3(reinterpret_cast<SUBSTRING*>(StrOut),
				StrInout.c_str() + begin_pos, StrInout.c_str() + end_pos);
			SUBSTRING_Trim(reinterpret_cast<SUBSTRING*>(StrOut));
			break;
		case 1:
			assert(StrOut);
			*reinterpret_cast<std::string*>(StrOut) 
				= boost::trim_copy(Utils::CopyToNewString(begin + 1, end));
			break;
		}
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
	return ExtractStringInternal(StrInout, 
		reinterpret_cast<void*>(&StrOut), 1, Pos, SQUARE_BEGIN_PRED, SQUARE_END_PRED, P);
}

size_t Parentheses::ExtractStringFromRound(std::string& StrInout, std::string& StrOut, size_t Pos, Params P)
{
	return ExtractStringInternal(StrInout, 
		reinterpret_cast<void*>(&StrOut), 1, Pos, ROUND_BEGIN_PRED, ROUND_END_PRED, P);
}

size_t Parentheses::ExtractStringFromBraces(std::string& StrInout, std::string& StrOut, size_t Pos, Params P)
{
	return ExtractStringInternal(StrInout, 
		reinterpret_cast<void*>(&StrOut), 1, Pos, BRACES_BEGIN_PRED, BRACES_END_PRED, P);
}



/******************************
* ExtractStringFrom__(std::string& StrInout, Params P)
*******************************/
size_t Parentheses::ExtractStringFromSquare(std::string& StrInout, size_t Pos, Params P)
{
	return ExtractStringInternal(StrInout, NULL, -1, Pos, SQUARE_BEGIN_PRED, SQUARE_END_PRED, P);
}

size_t Parentheses::ExtractStringFromRound(std::string& StrInout, size_t Pos, Params P)
{
	return ExtractStringInternal(StrInout, NULL, -1, Pos, ROUND_BEGIN_PRED, ROUND_END_PRED, P);
}

size_t Parentheses::ExtractStringFromBraces(std::string& StrInout, size_t Pos, Params P)
{
	return ExtractStringInternal(StrInout, NULL, -1, Pos, BRACES_BEGIN_PRED, BRACES_END_PRED, P);
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
	return ExtractStringInternal(StrInout, &StrOut, 1, Pos, AnyBeginPred, AnyEndPred, P);
}

/******************************
* ExtractStringFromAny(std::string& StrInout, Params P)
*******************************/
size_t Parentheses::ExtractStringFromAny(std::string& StrInout, size_t Pos, Params P)
{
	return ExtractStringInternal(StrInout, NULL, -1, Pos, AnyBeginPred, AnyEndPred, P);
}


/******************************
* ConstantExtractStringFromSquare(very lightweight version)
* without copying and memory allocation
*******************************/
size_t Parentheses::ConstantExtractStringFromSquare(const std::string& StrIn, SUBSTRING& StrOut, size_t Pos, Params P)
{
	P.ForbidModifyInoutString = true;
	return ExtractStringInternal(const_cast<std::string&>(StrIn), 
		&StrOut, 0, Pos, SQUARE_BEGIN_PRED, SQUARE_END_PRED, P);
}
size_t Parentheses::ConstantExtractStringFromRound(const std::string& StrIn, SUBSTRING& StrOut, size_t Pos, Params P)
{
	P.ForbidModifyInoutString = true;
	return ExtractStringInternal(const_cast<std::string&>(StrIn), 
		&StrOut, 0, Pos, ROUND_BEGIN_PRED, ROUND_END_PRED, P);
}
size_t Parentheses::ConstantExtractStringFromBraces(const std::string& StrIn, SUBSTRING& StrOut, size_t Pos, Params P)
{
	P.ForbidModifyInoutString = true;
	return ExtractStringInternal(const_cast<std::string&>(StrIn), 
		&StrOut, 0, Pos, BRACES_BEGIN_PRED, BRACES_END_PRED, P);
}
size_t Parentheses::ConstantExtractStringFromAny(const std::string& StrIn, SUBSTRING& StrOut, size_t Pos, Params P)
{
	P.ForbidModifyInoutString = true;
	return ExtractStringInternal(const_cast<std::string&>(StrIn), 
		&StrOut, 0, Pos, AnyBeginPred, AnyEndPred, P);
}

