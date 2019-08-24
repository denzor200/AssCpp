#include "stdafx.h"
#include "AssTime.h"
#include <iomanip>
#include <sstream>

#include "AssErrors.h"

using namespace ASS;

#define CHECK_TIME_VALUE(TYPE, VALUE, MAX) \
	if (VALUE > MAX) \
		throw AssTimeInvalidValue(TYPE, VALUE);

#define CHECK_MILISECONDS_VALUE(VALUE)	{CHECK_TIME_VALUE(AssTimeInvalidValue::ValueType::Miliseconds, VALUE, 999);}
#define CHECK_SECONDS_VALUE(VALUE)		{CHECK_TIME_VALUE(AssTimeInvalidValue::ValueType::Seconds, VALUE, 59);}
#define CHECK_MINUTES_VALUE(VALUE)		{CHECK_TIME_VALUE(AssTimeInvalidValue::ValueType::Minutes, VALUE, 59)}
#define CHECK_HOURS_VALUE(VALUE)		{CHECK_TIME_VALUE(AssTimeInvalidValue::ValueType::Hours, VALUE, 9)}

AssTime::AssTime(uint32_t Miliseconds)
{
	m_Hours = Miliseconds / (60 * 60000);		CHECK_HOURS_VALUE(m_Hours);
	Miliseconds -= m_Hours;
	m_Minutes = Miliseconds / 60000;			CHECK_MINUTES_VALUE(m_Minutes);
	Miliseconds -= m_Minutes;
	m_Seconds = Miliseconds / 1000;				CHECK_SECONDS_VALUE(m_Seconds);
	m_Miliseconds = Miliseconds - m_Seconds;	CHECK_MILISECONDS_VALUE(m_Miliseconds);
}

AssTime::AssTime(uint32_t Hours, uint32_t Minutes, uint32_t Seconds) :
	m_Hours(Hours),
	m_Minutes(Minutes),
	m_Seconds(Seconds),
	m_Miliseconds(0)
{
	CHECK_HOURS_VALUE			(m_Hours);
	CHECK_MINUTES_VALUE			(m_Minutes);
	CHECK_SECONDS_VALUE			(m_Seconds);
	CHECK_MILISECONDS_VALUE		(m_Miliseconds);
}

AssTime::AssTime(uint32_t Hours, uint32_t Minutes, uint32_t Seconds, uint32_t Miliseconds) :
	m_Hours(Hours),
	m_Minutes(Minutes),
	m_Seconds(Seconds),
	m_Miliseconds(Miliseconds)
{
	CHECK_HOURS_VALUE			(m_Hours);
	CHECK_MINUTES_VALUE			(m_Minutes);
	CHECK_SECONDS_VALUE			(m_Seconds);
	CHECK_MILISECONDS_VALUE		(m_Miliseconds);
}

uint32_t AssTime::TotalMiliseconds() const noexcept
{
	assert(m_Miliseconds < 1000);
	assert(m_Seconds < 60);
	assert(m_Minutes < 60);
	assert(m_Hours < 10); // ass поддерживает только одну цифру в часе
	// TODO: выясни, какие есть еще ограничения на хранение времени
	return m_Miliseconds + m_Seconds * 1000 + m_Minutes * 60000 + m_Hours * (60 * 60000);
}

void AssTime::SetHours(uint32_t Hours)
{
	CHECK_HOURS_VALUE(Hours);
	m_Hours = Hours;
}

void AssTime::SetMinutes(uint32_t Minutes)
{
	CHECK_MINUTES_VALUE(Minutes);
	m_Minutes = Minutes;
}

void AssTime::SetSeconds(uint32_t Seconds)
{
	CHECK_SECONDS_VALUE(Seconds);
	m_Seconds = Seconds;
}

void AssTime::SetMiliseconds(uint32_t Miliseconds)
{
	CHECK_MILISECONDS_VALUE(Miliseconds);
	m_Miliseconds = Miliseconds;
}

// TODO: optimize this
static uint32_t StringToMiliseconds(std::string Str)
{
	switch (Str.size())
	{
	case 1:
		Str += "00";
		break;
	case 2:
		Str += "0";
		break;
	}
	Str.resize(3);
	return std::stoi(Str);
}

void AssTime::Parse(const std::string& Value)
{
	static const char Sym[] = {
		':',
		':',
		'.'
	};

	std::vector<std::string> xResults;
	uint8_t SplitCounter = 0;
	// TODO: make it without boost
	boost::split(xResults, Value, [&SplitCounter](char C)
	{
		if (sizeof(Sym) <= SplitCounter)
		{
			return false;
		}

		if (C == Sym[SplitCounter])
		{
			++SplitCounter;
			return true;
		}
		return false;
	});

	if (xResults.size() < 3)
		throw AssTimeInvalidString();

	bool Status = false;
	Utils::StoiPassAllEceptionsWrapper([this, &Status, &xResults]() {
		AssTime TempTime;
		TempTime.SetHours(std::stoi(xResults[0]));
		TempTime.SetMinutes(std::stoi(xResults[1]));
		TempTime.SetSeconds(std::stoi(xResults[2]));

		if (xResults.size() >= 4)
			TempTime.SetMiliseconds(StringToMiliseconds(xResults[3]));

		std::swap(*this, TempTime);
		Status = true;
	});
	
	if (!Status)
		throw AssTimeInvalidString();
}

std::string AssTime::Print(WriteParams Params) const
{
	std::stringstream Stream;
	Stream << std::setfill('0')
		<< std::setw(1) << m_Hours
		<< ':'
		<< std::setw(2) << m_Minutes
		<< ':'
		<< std::setw(2) << m_Seconds;

	if (Params.WithMiliseconds)
	{
		uint32_t Miliseconds = m_Miliseconds / 10;
		Stream << std::setfill('0')
			<< '.'
			<< std::setw(2) << Miliseconds;
	}
	return Stream.str();
}

