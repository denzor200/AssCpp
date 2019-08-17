#include "stdafx.h"
#include "AssDialogString.h"
#include <regex>

#define FLAG_TRUE(f)	(f>0)
#define FLAG_FALSE(f)	(f==0)

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

void AssDialogString::ParseTo(SegmentsType& Out, const char* Value)
{
	
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

