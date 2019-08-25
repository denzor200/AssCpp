#include "stdafx.h"
#include "AssScript.h"
#include "EncodingUtils.h"
#include "SubString.h"
#include <fstream>
#include <iostream>

#include <boost/optional/optional_io.hpp>

#include "AssErrors.h"

using namespace ASS;

namespace AssEnums
{
	boost::optional<AssEvent::AssEventType> ReadAssEventType(const std::string& Value)
	{
		using AssEventType = AssEvent::AssEventType;
		if (Value == "Dialogue")
			return AssEventType::Dialogue;
		else if (Value == "Comment")
			return AssEventType::Comment;
		else if (Value == "Picture")
			return AssEventType::Picture;
		else if (Value == "Sound")
			return AssEventType::Sound;
		else if (Value == "Movie")
			return AssEventType::Movie;
		else if (Value == "Command")
			return AssEventType::Command;
		return {};
	}

	boost::optional<AssSectionScriptInfo::ScriptType> ReadScriptType(const std::string& Value)
	{
		using ScriptType = AssSectionScriptInfo::ScriptType;
		if (Value == "v4.00+")
			return ScriptType::V400plus;
		return { };
	}

	boost::optional<AssSectionScriptInfo::Collisions> ReadCollisions(const std::string& Value)
	{
		using Collisions = AssSectionScriptInfo::Collisions;
		if (Value == "Normal")
			return Collisions::Normal;
		else if (Value == "Reverse")
			return Collisions::Reverse;
		return {};
	}

	boost::optional<AssSectionScriptInfo::WrapStyle> ReadWrapStyle(const std::string& Value)
	{
		using WrapStyle = AssSectionScriptInfo::WrapStyle;
		if (Value == "0")
			return WrapStyle::ZERO;
		else if (Value == "1")
			return WrapStyle::FIRST;
		else if (Value == "2")
			return WrapStyle::SECOND;
		else if (Value == "3")
			return WrapStyle::THIRD;
		return {};
	}

	static std::string PrintAssEventType(AssEvent::AssEventType Type)
	{
		using AssEventType = AssEvent::AssEventType;
		switch (Type)
		{
		case AssEventType::Dialogue:
			return "Dialogue";
		case AssEventType::Comment:
			return "Comment";
		case AssEventType::Picture:
			return "Picture";
		case AssEventType::Sound:
			return "Sound";
		case AssEventType::Movie:
			return "Movie";
		case AssEventType::Command:
			return "Command";
		}
		std::cerr << "Invalid 'AssEventType' value" << std::endl;
		assert(0);
		return "";
	}

	static std::string PrintScriptType(AssSectionScriptInfo::ScriptType Type)
	{
		using ScriptType = AssSectionScriptInfo::ScriptType;
		switch (Type)
		{
		case ScriptType::V400plus:
			return "v4.00+";
		}
		std::cerr << "Invalid 'ScriptType' value" << std::endl;
		assert(0);
		return "";
	}

	static std::string PrintCollisions(AssSectionScriptInfo::Collisions C)
	{
		using Collisions = AssSectionScriptInfo::Collisions;
		switch (C)
		{
		case Collisions::Normal:
			return "Normal";
		case Collisions::Reverse:
			return "Reverse";
		}
		std::cerr << "Invalid 'Collisions' value" << std::endl;
		assert(0);
		return "";
	}

	static std::string PrintWrapStyle(AssSectionScriptInfo::WrapStyle W)
	{
		using WrapStyle = AssSectionScriptInfo::WrapStyle;
		switch (W)
		{
		case WrapStyle::ZERO:
			return "0";
		case WrapStyle::FIRST:
			return "1";
		case WrapStyle::SECOND:
			return "2";
		case WrapStyle::THIRD:
			return "3";
		}
		std::cerr << "Invalid 'WrapStyle' value" << std::endl;
		assert(0);
		return "";
	}
};

namespace AssScriptUtils
{
	// Прокрутить до первой не пустой строки
	// TODO: optimize this
	template <typename STREAM_T>
	static boost::optional<std::string> ScrollToNonempty(STREAM_T& Stream)
	{
		std::string LineStr;
		while (std::getline(Stream, LineStr))
		{
			boost::trim(LineStr);
			if (!LineStr.empty())
				return LineStr;
		}
		return {};
	}
	static bool IsSectionName(const std::string& Value)
	{
		// TODO: implement this
		return false;
	}
	static boost::optional<std::pair<std::string, std::string>> LineExtractKeyvalue(const std::string& LineStr)
	{
		// TODO: implement this
		return {};
	}

};

namespace AssSerialize
{

	template <typename STREAM_T>
	void AssStyleWrite(const AssStyle& E, STREAM_T& Stream)
	{
		// TODO: implement this
	}

	template <typename STREAM_T>
	void AssEventWrite(const AssEvent& E, STREAM_T& Stream)
	{
		Stream << AssEnums::PrintAssEventType(E.Type)
			<< ": " << E.Layer
			<< "," << E.Start.Print()
			<< "," << E.End.Print()
			<< "," << E.Style
			<< "," << E.Name
			<< "," << E.MarginL
			<< "," << E.MarginR
			<< "," << E.MarginV
			<< "," << E.Effect
			<< "," << E.Text << std::endl;
	}
};

// Отсутствуют гарантии безопасности исключений(они здесь и не нужны)
boost::optional<std::string> AssSectionScriptInfo::Read(std::istream& Stream)
{
	boost::optional<std::string> ReturnValue;
	std::string LineStr;
	bool Initialized = false;
	while (std::getline(Stream, LineStr))
	{
		boost::trim(LineStr); // На всякий случай
		// Пустые строки нам не нужны
		if (LineStr.empty())
			continue;
		// Другая секция.. Достаточно, заканчиваем
		if (AssScriptUtils::IsSectionName(LineStr))
		{
			ReturnValue = LineStr;
			break;
		}

		const auto& KV = AssScriptUtils::LineExtractKeyvalue(LineStr);
		if (KV)
		{
			const auto& first = KV.get().first;
			const auto& second = KV.get().second;
			// Обрабатываем, если что-то получилось извлечь
			// Все что странное будет игнорироваться(но ScriptType должен быть. И должен соответствовать поддерживаемой версии)
			if (first == "ScriptType")
			{
				auto ScriptType = AssEnums::ReadScriptType(second);
				if (ScriptType)
				{
					m_ScriptType = ScriptType.get();
					Initialized = true;
				}
			}
			else if (first == "Collisions")
				m_Collisions = AssEnums::ReadCollisions(second);
			else if (first == "PlayResX")
				Utils::StoiPassAllEceptionsWrapper([&]() {m_PlayResX = std::stoi(second); });
			else if (first == "PlayResY")
				Utils::StoiPassAllEceptionsWrapper([&]() {m_PlayResY = std::stoi(second); });
			else if (first == "PlayDepth")
				Utils::StoiPassAllEceptionsWrapper([&]() {m_PlayDepth = std::stoi(second); });
			else if (first == "Timer")
				Utils::StoiPassAllEceptionsWrapper([&]() {m_Timer = std::stod(second); });
			else if (first == "WrapStyle")
				m_WrapStyle = AssEnums::ReadWrapStyle(second);
		}
	}

	// Проверяем, перед тем как закончить..
	if (!Initialized)
		throw AssScriptInvalidScriptType();
	// all right
	return ReturnValue;
}

void AssSectionScriptInfo::Write(std::ostream& Stream) const
{
	assert(m_ScriptType != ScriptType::UNKNOWN); // not initialized

	Stream << "ScriptType: " << AssEnums::PrintScriptType(m_ScriptType) << std::endl;
	// TODO: test output boost::optional
	if (m_Collisions)
		Stream << "Collisions: " << AssEnums::PrintCollisions(m_Collisions.get()) << std::endl;
	if (m_PlayResX)
		Stream << "PlayResX: " << m_PlayResX << std::endl;
	if (m_PlayResY)
		Stream << "PlayResY: " << m_PlayResY << std::endl;
	if (m_PlayDepth)
		Stream << "PlayDepth: " << m_PlayDepth << std::endl;
	if (m_Timer)
		Stream << "Timer: " << m_Timer << std::endl;
	if (m_WrapStyle)
		Stream << "WrapStyle: " << AssEnums::PrintWrapStyle(m_WrapStyle.get()) << std::endl;

	if (!Title.empty())
		Stream << "Title: " << Title << std::endl;
	if (!OriginalScript.empty())
		Stream << "OriginalScript: " << OriginalScript << std::endl;
	if (!OriginalTranslation.empty())
		Stream << "OriginalTranslation: " << OriginalTranslation << std::endl;
	if (!OriginalEditing.empty())
		Stream << "OriginalEditing: " << OriginalEditing << std::endl;
	if (!OriginalTiming.empty())
		Stream << "OriginalTiming: " << OriginalTiming << std::endl;
	if (!SynchPoint.empty())
		Stream << "SynchPoint: " << SynchPoint << std::endl;
	if (!ScriptUpdatedBy.empty())
		Stream << "ScriptUpdatedBy: " << ScriptUpdatedBy << std::endl;
	if (!UpdateDetails.empty())
		Stream << "UpdateDetails: " << UpdateDetails << std::endl;
}

// Отсутствуют гарантии безопасности исключений(они здесь и не нужны)
boost::optional<std::string> AssSectionV4Styles::Read(std::istream& Stream)
{
	// TODO: implement this
	return {};
}

void AssSectionV4Styles::Write(std::ostream& Stream) const
{
	Stream << "Format: "
		"Name, "
		"Fontname, "
		"Fontsize, "
		"PrimaryColour, "
		"SecondaryColour, "
		"OutlineColour, "
		"BackColour, "
		"Bold, "
		"Italic, "
		"Underline, "
		"StrikeOut, "
		"ScaleX, "
		"ScaleY, "
		"Spacing, "
		"Angle, "
		"BorderStyle, "
		"Outline, "
		"Shadow, "
		"Alignment, "
		"MarginL, "
		"MarginR, "
		"MarginV, "
		"Encoding" << std::endl;

	for (const auto& Style : m_Styles)
	{
		assert(Style.first == Style.second.Name);
		AssSerialize::AssStyleWrite(Style.second, Stream);
	}
}

#define FORMAT_INVALID_ID ((uint8_t)-1)

struct EventsFormatInfo
{
	uint8_t Layer = FORMAT_INVALID_ID;
	uint8_t Start = FORMAT_INVALID_ID;
	uint8_t End = FORMAT_INVALID_ID;
	uint8_t Style = FORMAT_INVALID_ID;
	uint8_t Name = FORMAT_INVALID_ID;
	uint8_t MarginL = FORMAT_INVALID_ID;
	uint8_t MarginR = FORMAT_INVALID_ID;
	uint8_t MarginV = FORMAT_INVALID_ID;
	uint8_t Effect = FORMAT_INVALID_ID;
	uint8_t Text = FORMAT_INVALID_ID; // Всегда должен быть последним

	EventsFormatInfo() = default;
};

#define FORMAT_LAYER "Layer"
#define FORMAT_START "Start"
#define FORMAT_END "End"
#define FORMAT_STYLE "Style"
#define FORMAT_NAME "Name"
#define FORMAT_MARGINL "MarginL"
#define FORMAT_MARGINR "MarginR"
#define FORMAT_MARGINV "MarginV"
#define FORMAT_EFFECT "Effect"
#define FORMAT_TEXT "Text"

// TODO: реализовать возможность задавать имена для 'Format' из вне
// Это важно для Biff
static void ParseFormat(EventsFormatInfo& F, const std::string& Line)
{
	auto ParseToken = [&F, &Line](uint8_t ID, size_t B, size_t E)
	{
		auto SetFieldID = [](const char* Field, uint8_t& Value, uint8_t ID)
		{
			if (FORMAT_INVALID_ID != Value)
				throw AssEventsFieldIdAlreadyDefined(Field);
			Value = ID;
		};

		SUBSTRING Str;
		SUBSTRING_Init3(&Str, Line.c_str() + B, Line.c_str() + E);
		SUBSTRING_Trim(&Str);

		if (0 == SUBSTRING_CompareWitchConst(Str, FORMAT_LAYER))
			SetFieldID(FORMAT_LAYER, F.Layer, ID);
		else if (0 == SUBSTRING_CompareWitchConst(Str, FORMAT_START))
			SetFieldID(FORMAT_START, F.Start, ID);
		else if (0 == SUBSTRING_CompareWitchConst(Str, FORMAT_END))
			SetFieldID(FORMAT_END, F.End, ID);
		else if (0 == SUBSTRING_CompareWitchConst(Str, FORMAT_STYLE))
			SetFieldID(FORMAT_STYLE, F.Style, ID);
		else if (0 == SUBSTRING_CompareWitchConst(Str, FORMAT_NAME))
			SetFieldID(FORMAT_NAME, F.Name, ID);
		else if (0 == SUBSTRING_CompareWitchConst(Str, FORMAT_MARGINL))
			SetFieldID(FORMAT_MARGINL, F.MarginL, ID);
		else if (0 == SUBSTRING_CompareWitchConst(Str, FORMAT_MARGINR))
			SetFieldID(FORMAT_MARGINR, F.MarginR, ID);
		else if (0 == SUBSTRING_CompareWitchConst(Str, FORMAT_MARGINV))
			SetFieldID(FORMAT_MARGINV, F.MarginV, ID);
		else if (0 == SUBSTRING_CompareWitchConst(Str, FORMAT_EFFECT))
			SetFieldID(FORMAT_EFFECT, F.Effect, ID);
		else if (0 == SUBSTRING_CompareWitchConst(Str, FORMAT_TEXT))
			SetFieldID(FORMAT_TEXT, F.Text, ID);
	};

	size_t BPos = 0;
	uint8_t ID = 0;
	for (size_t EPos = Line.find(','); std::string::npos != EPos; EPos = Line.find(',', EPos))
	{
		ParseToken(ID, BPos, EPos);
		BPos = EPos + 1;
		ID++;
	}
	ParseToken(ID, BPos, Line.size());
}

static void ParseDialogue(AssEvent& E, const EventsFormatInfo& F, const std::string& Line)
{
	
}

// Отсутствуют гарантии безопасности исключений(они здесь и не нужны)
boost::optional<std::string> AssSectionEvents::Read(std::istream& Stream)
{
	boost::optional<EventsFormatInfo> Format;
	std::string LineStr;
	while (std::getline(Stream, LineStr))
	{
		boost::trim(LineStr); // На всякий случай
		if (AssScriptUtils::IsSectionName(LineStr))
			return LineStr;
		const auto& KV = AssScriptUtils::LineExtractKeyvalue(LineStr);
		if (KV)
		{
			const auto& first = KV.get().first;
			const auto& second = KV.get().second;
			if (first == "Format")
			{
				Format = EventsFormatInfo();
				ParseFormat(Format.get(), second);
			}
			else if (first == "Dialogue")
			{
				if (!Format)
					throw AssEventsFormatLineNotFound();
				AssEvent NewEvent;
				// TODO: implement this
			}
			else if (first == "Comment")
			{
				// TODO: implement this
			}
			else if (first == "Picture")
			{
				// TODO: implement this
			}
			else if (first == "Sound")
			{
				// TODO: implement this
			}
			else if (first == "Movie")
			{
				// TODO: implement this
			}
			else if (first == "Command")
			{
				// TODO: implement this
			}

		}
	}
	return {};
}

void AssSectionEvents::Write(std::ostream& Stream) const
{
	Stream << "Format: "
		"Layer, "
		"Start, "
		"End, "
		"Style, "
		"Name, "
		"MarginL, "
		"MarginR, "
		"MarginV, "
		"Effect, "
		"Text" << std::endl;

	for (const auto& Event : m_Events)
	{
		AssSerialize::AssEventWrite(Event, Stream);
	}
}

AssScript::AssScript(const std::string& FileName)
{
	std::ifstream ifs(FileName.c_str()/*, std::ios::binary*/);
	// TODO: catch exception
	// TODO: убедись, что отказ от конвертации в std::stringstream прошел гладко
	EncodingUtils::PrepareStream(ifs);
	ParseTo(m_Impl, ifs);
}


AssScript::~AssScript()
{
}

void AssScript::Parse(std::istream& Stream)
{
	AssImpl TempImpl;
	ParseTo(TempImpl, Stream);
	// TODO: убедись, что swap отрабатывает быстро
	std::swap(m_Impl, TempImpl);
}

std::string AssScript::Print() const
{
	// TODO: implement this
	return "";
}

void AssScript::Read(const std::string& FileName)
{
	// TODO: implement this
}

void AssScript::Write(const std::string& FileName) const
{
	// TODO: implement this
}

#define SCRIPT_INFO_SECTION		"[Script Info]"
#define V4PLUS_STYLES_SECTION	"[V4+ Styles]"
#define EVENTS_SECTION			"[Events]"

void AssScript::ParseTo(AssImpl& Impl, std::istream& Stream)
{
	boost::optional<std::string> LineStr;

	// Первая секция обязана быть Script Info
	LineStr = AssScriptUtils::ScrollToNonempty(Stream);
	if (!LineStr || LineStr.get() != SCRIPT_INFO_SECTION)
		throw AssScriptCantFindScriptInfo();
	LineStr = Impl.ScriptInfo.Read(Stream);

	// Проходим по остальным секциям
	// Тут порядок не важен
	// TODO: Разделены ли секции между собой пробелами?? Гарантирует ли это стандарт?
	while (LineStr)
	{
		if (LineStr.get() == V4PLUS_STYLES_SECTION)
			LineStr = Impl.V4Styles.Read(Stream);
		else if (LineStr.get() == EVENTS_SECTION)
		{
			LineStr = Impl.Events.Read(Stream);
		}
	}
}

void AssScript::Write(std::ostream& Stream) const
{
	Stream << SCRIPT_INFO_SECTION << std::endl;
	m_Impl.ScriptInfo.Write(Stream);

	if (!m_Impl.V4Styles.AllStyles().empty())
	{
		Stream << V4PLUS_STYLES_SECTION << std::endl;
		m_Impl.V4Styles.Write(Stream);
	}

	if (!m_Impl.Events.AllEvents().empty())
	{
		Stream << EVENTS_SECTION << std::endl;
		m_Impl.Events.Write(Stream);
	}
}


