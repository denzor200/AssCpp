#include "stdafx.h"
#include "AssScript.h"
#include <sstream>

#include <boost\optional.hpp>

using namespace ASS;

extern std::stringstream readFile(std::string path);

namespace AssScriptUtils
{
	// Прокрутить до первой не пустой строки
	// TODO: optimize this
	static boost::optional<std::string> ScrollToNonempty(std::stringstream& ss)
	{
		std::string LineStr;
		while (std::getline(ss, LineStr))
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


	static boost::optional<std::string> ParseScriptInfo(AssSectionScriptInfo& SI, std::stringstream& ss)
	{
		
		return {};
	}
	static boost::optional<std::string> ParseV4Styles(AssSectionV4Styles& V4S, std::stringstream& ss)
	{
		
		return {};
	}
	static boost::optional<std::string> ParseEvents(AssSectionEvents& E, std::stringstream& ss)
	{
		
		return {};
	}
};

AssScript::AssScript(const std::string& FileName)
{
	std::stringstream ss = readFile(FileName);
	ParseTo(m_Impl, ss);
}
AssScript::AssScript(std::stringstream& ss)
{
	ParseTo(m_Impl, ss);
}


AssScript::~AssScript()
{
}

void AssScript::Parse(std::stringstream& ss)
{
	AssImpl TempImpl;
	ParseTo(TempImpl, ss);
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
	
}

void AssScript::Write(const std::string& FileName) const
{

}

#define SCRIPT_INFO_SECTION		"[Script Info]"
#define V4PLUS_STYLES_SECTION	"[V4+ Styles]"
#define EVENTS_SECTION			"[Events]"

void AssScript::ParseTo(AssImpl& Impl, std::stringstream& ss)
{
	boost::optional<std::string> LineStr;

	// Первая секция обязана быть Script Info
	LineStr = AssScriptUtils::ScrollToNonempty(ss);
	if (!LineStr || LineStr.get() != SCRIPT_INFO_SECTION)
		throw AssScriptCantFindScriptInfo();
	LineStr = AssScriptUtils::ParseScriptInfo(Impl.ScriptInfo, ss);

	// Проходим по остальным секциям
	// Тут порядок не важен
	// TODO: Разделены ли секции между собой пробелами?? Гарантирует ли это стандарт?
	while (LineStr)
	{
		if (LineStr.get() == V4PLUS_STYLES_SECTION)
			LineStr = AssScriptUtils::ParseV4Styles(Impl.V4Styles, ss);
		else if (LineStr.get() == EVENTS_SECTION)
		{
			LineStr = AssScriptUtils::ParseEvents(Impl.Events, ss);
		}
	}
}

