#include "stdafx.h"
#include "AssDialogString.h"
#include "Parentheses.h"
#include <regex>

#include <iostream>

// for "unsigned" flags
#define FLAG_TRUE(f)	(f>0)
#define FLAG_FALSE(f)	(f==0)
#define FLAG_ON(f)		{++f;}
#define FLAG_OFF(f)		{if (f>0) {--f;}}

#define END_POS(P,V)	((std::string::npos != P) ? P : V.size())

#define NEED_ASS_DIALOG_STRING_DEBUG

AssDialogString::AssDialogString(const char* Value)
{
	ParseTo(m_Segments, Value);
}


AssDialogString::~AssDialogString()
{
}

void AssDialogString::Parse(const char* Value)
{
	SegmentsType TempSections;
	ParseTo(TempSections, Value);
	std::swap(m_Segments, TempSections);
}

namespace AssDialogStringUtils
{
	static bool ParseToken(AssDialogString::SegmentParams& Params, const std::string& Token)
	{
		/* Bold */
		if (Token == "\\b1")
		{
			FLAG_ON(Params.Bold);
			return true;
		}
		if (Token == "\\b0")
		{
			FLAG_OFF(Params.Bold);
			return true;
		}

		/* Underline */
		if (Token == "\\u1")
		{
			FLAG_ON(Params.Underline);
			return true;
		}
		if (Token == "\\u0")
		{
			FLAG_OFF(Params.Underline);
			return true;
		}

		/* Italic */
		if (Token == "\\i1")
		{
			FLAG_ON(Params.Italic);
			return true;
		}
		if (Token == "\\i0")
		{
			FLAG_OFF(Params.Italic);
			return true;
		}

#ifdef NEED_ASS_DIALOG_STRING_DEBUG
		std::cout << "Unexpected token: " << Token << std::endl;
#endif // NEED_ASS_DIALOG_STRING_DEBUG

		return false;
	}
	static bool ParseTokens(
		AssDialogString::SegmentParams& Params,
		const std::string& Tokens,
		std::string* _Unexpected,
		size_t* _UnexpectedFilledSize)
	{
		// TODO: vector<string> ??
		std::string Unexpected = "";
		size_t UnexpectedFilledSize = 0;

		size_t ExpectedSize = 0;
		size_t LastDivPos = 0;

		Unexpected += "{";

		size_t DivPos = Tokens.find('\\');
		if (0 != DivPos)
		{
			// unknown trash in begin..
			// its not token definitely
			size_t EndPos = END_POS(DivPos, Tokens);
			Unexpected += Utils::CopyToNewString(Tokens, 0, EndPos);
			++UnexpectedFilledSize;
		}

		while (std::string::npos != DivPos)
		{
			size_t NextDivPos = Tokens.find('\\', DivPos + 1);
			size_t EndPos = END_POS(NextDivPos, Tokens);
			std::string Token = Utils::CopyToNewString(Tokens, DivPos, EndPos);
			boost::trim(Token); // на всякий случай.. Вдруг токены перечислены через пробел??
			if (!AssDialogStringUtils::ParseToken(Params, Token))
			{
				Unexpected += Token;
				// Если тег был разобран успешно, но в нем были пустые токены, которые сформировали на выход тег вида "{\\\\}"
				// То класть его обратно нет смысла
				// Но если в теге был хоть один не опознанный токен, который тоже идет на выход, то еще нормально, оставляем так..
				if (Token != "\\")
					++UnexpectedFilledSize;
			}
			else
				++ExpectedSize;

			DivPos = NextDivPos;
		}

		Unexpected += "}";

		assert(_Unexpected);
		assert(_UnexpectedFilledSize);
		*_Unexpected = std::move(Unexpected);
		*_UnexpectedFilledSize = std::move(UnexpectedFilledSize);
		return (0 != ExpectedSize);
	}
};

void AssDialogString::ParseTo(SegmentsType& Out, std::string Value)
{
	size_t LastPos = 0;
	SegmentParams Params;
	// нельзя изменять строку, не имея на это каких либо веских оснований
	// По этому сначала вызывается константная версия Parentheses-процедуры
	// И только в конце, когда мы смогли распознать хоть один токен, только тогда мы можем вызывать не константную процедуру

#define CEXTRACT(...)	(Parentheses::ConstantExtractStringFromBraces(__VA_ARGS__))
#define EXTRACT(...)	(Parentheses::ExtractStringFromBraces(__VA_ARGS__))

	std::string Tokens;
	for (size_t Pos = CEXTRACT(Value, Tokens);
		Parentheses::NOT_FOUND != Pos; 
		Pos = CEXTRACT(Value, Tokens, Pos))
	{
		bool Status = false;

		// Preparing for next iteration..
		std::string Unexpected = "";
		size_t UnexpectedFilledSize = 0;
		SegmentParams TempParams;
		if (AssDialogStringUtils::ParseTokens(TempParams, Tokens, &Unexpected, &UnexpectedFilledSize))
		{
			Segment NewSegment;
			NewSegment.Params = Params;
			NewSegment.Value = Utils::CopyToNewString(Value, LastPos, Pos);
			// NewSegment еще не сформирован, рано его заносить..
			// Возможно в него пойдет Unexpected

			Params = TempParams;

			// all right
			size_t SamePos = EXTRACT(Value, Pos);
			assert(SamePos == Pos);
			if (0 != UnexpectedFilledSize)
			{
				// Кладем это все на место, не теряем
				NewSegment.Value += Unexpected;
			}
			if (!NewSegment.Value.empty()) // ignore empty string
				Out.push_back(std::move(NewSegment));
			Status = true;
		}
		else
		{
			// Мы здесь вообще ничего не трогали, смещаемся
			Pos += Tokens.size() + 2; // + ("{}").size()
		}

		if (Status)
			LastPos = Pos;
	}

#undef CEXTRACT
#undef EXTRACT

	// tail..
	size_t ValueSize = Value.size();
	if (ValueSize != 0)
	{
		if (ValueSize - 1 != LastPos) // ignore empty string
		{
			Segment NewSegment;
			NewSegment.Params = Params;
			NewSegment.Value = Utils::CopyToNewString(Value, LastPos, ValueSize );
			Out.push_back(std::move(NewSegment));
		}
	}

}

std::string AssDialogString::Print() const
{
	std::string Value = "";
	SegmentParams LastParams;

	for (const auto& Segment : m_Segments)
	{
		const auto& SegmentParams = Segment.Params;
		// detect changes..
		if (FLAG_TRUE(SegmentParams.Bold) && !FLAG_TRUE(LastParams.Bold))
		{
			Value += "{\\b1}";
		}
		if (!FLAG_TRUE(SegmentParams.Bold) && FLAG_TRUE(LastParams.Bold))
			Value += "{\\b0}";

		if (FLAG_TRUE(SegmentParams.Italic) && !FLAG_TRUE(LastParams.Italic))
			Value += "{\\i1}";
		if (!FLAG_TRUE(SegmentParams.Italic) && FLAG_TRUE(LastParams.Italic))
		{
			Value += "{\\i0}";
		}

		if (FLAG_TRUE(SegmentParams.Underline) && !FLAG_TRUE(LastParams.Underline))
			Value += "{\\u1}";
		if (!FLAG_TRUE(SegmentParams.Underline) && FLAG_TRUE(LastParams.Underline))
		{
			Value += "{\\u0}";
		}

		Value += Segment.Value;

		LastParams.Bold = SegmentParams.Bold;
		LastParams.Italic = SegmentParams.Italic;
		LastParams.Underline = SegmentParams.Underline;
	}

	// tail..
	// это совсем не обязательно..
	/*if (FLAG_TRUE(LastParams.Bold))
		Value += "{\\b0}";
	if (FLAG_TRUE(LastParams.Italic))
		Value += "{\\i0}";
	if (FLAG_TRUE(LastParams.Underline))
		Value += "{\\u0}";*/

	return Value;
}

