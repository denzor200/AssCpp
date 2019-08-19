#include "stdafx.h"
#include "AssScript.h"
#include "EncodingUtils.h"
#include <fstream>

#include <boost\optional.hpp>

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
};

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

