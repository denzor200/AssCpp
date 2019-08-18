#pragma once

#include <string>
#include <vector>

class AssDialogString
{
public:
	struct SegmentParams
	{
		unsigned Bold = 0;
		unsigned Italic = 0;
		unsigned Underline = 0;

		SegmentParams() = default;
	};
	struct Segment
	{
		std::string Value;
		SegmentParams Params;

		Segment() = default;
	};

private:
	using SegmentsType = std::vector<Segment>;
	using RSegmentsType = SegmentsType & ;
	using CRSegmentsType = const SegmentsType&;
	SegmentsType m_Segments;

public:
	AssDialogString(const char* Value);
	~AssDialogString();

	CRSegmentsType Segments() const;
	RSegmentsType Segments();

	void Parse(const char* Value);
	std::string Print() const;

private:
	static void ParseTo(SegmentsType& Out, std::string Value);
};


inline AssDialogString::CRSegmentsType AssDialogString::Segments() const
{
	return m_Segments;
}
inline AssDialogString::RSegmentsType AssDialogString::Segments()
{
	return m_Segments;
}
