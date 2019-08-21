#include "stdafx.h"
#include "AssScript.h"
#include "EncodingUtils.h"
#include <fstream>

#include <boost/optional/optional_io.hpp>

using namespace ASS;

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

	}

	template <typename STREAM_T>
	static boost::optional<std::string> ParseScriptInfo(AssSectionScriptInfo& SI, STREAM_T& Stream)
	{
		// TODO: implement this
		return {};
	}

	template <typename STREAM_T>
	static boost::optional<std::string> ParseV4Styles(AssSectionV4Styles& V4S, STREAM_T& Stream)
	{
		// TODO: implement this
		return {};
	}

	template <typename STREAM_T>
	static boost::optional<std::string> ParseEvents(AssSectionEvents& E, STREAM_T& Stream)
	{
		// TODO: implement this
		return {};
	}

	// TODO: тут нужна перегрузка оператора <<
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

	static std::string PrintAssEventTime(AssTime tm)
	{
		// TODO: implement this
		return "";
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
		Stream << AssScriptUtils::PrintAssEventType(E.Type)
			<< ": " << E.Layer
			<< "," << AssScriptUtils::PrintAssEventTime(E.Start)
			<< "," << AssScriptUtils::PrintAssEventTime(E.End)
			<< "," << E.Style
			<< "," << E.Name
			<< "," << E.MarginL
			<< "," << E.MarginR
			<< "," << E.MarginV
			<< "," << E.Effect
			<< "," << E.Text << std::endl;
	}
};

void AssSectionScriptInfo::Write(std::ostream& Stream) const
{
	assert(m_ScriptType != ScriptType::UNKNOWN); // not initialized

	Stream << "ScriptType: " << AssScriptUtils::PrintScriptType(m_ScriptType) << std::endl;
	// TODO: test output boost::optional
	if (m_Collisions)
		Stream << "Collisions: " << AssScriptUtils::PrintCollisions(m_Collisions.get()) << std::endl;
	if (m_PlayResX)
		Stream << "PlayResX: " << m_PlayResX << std::endl;
	if (m_PlayResY)
		Stream << "PlayResY: " << m_PlayResY << std::endl;
	if (m_PlayDepth)
		Stream << "PlayDepth: " << m_PlayDepth << std::endl;
	if (m_Timer)
		Stream << "Timer: " << m_Timer << std::endl;
	if (m_WrapStyle)
		Stream << "WrapStyle: " << AssScriptUtils::PrintWrapStyle(m_WrapStyle.get()) << std::endl;

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
	LineStr = AssScriptUtils::ParseScriptInfo(Impl.ScriptInfo, Stream);

	// Проходим по остальным секциям
	// Тут порядок не важен
	// TODO: Разделены ли секции между собой пробелами?? Гарантирует ли это стандарт?
	while (LineStr)
	{
		if (LineStr.get() == V4PLUS_STYLES_SECTION)
			LineStr = AssScriptUtils::ParseV4Styles(Impl.V4Styles, Stream);
		else if (LineStr.get() == EVENTS_SECTION)
		{
			LineStr = AssScriptUtils::ParseEvents(Impl.Events, Stream);
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


