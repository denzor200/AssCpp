#pragma once

#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/split.hpp>
#ifndef DISABLE_ICU
#include <unicode/unistr.h>
#endif // DISABLE_ICU

namespace Utils
{

	inline void TrimAll(std::vector<std::string>& Strings)
	{
		for (std::string& Str : Strings)
		{
			boost::trim(Str);
		}
	}

	inline void TrimMany(std::vector<std::string>& Strings, size_t Count)
	{
		size_t i = 0;
		for (std::string& Str : Strings)
		{
			if (i >= Count)
				break;
			boost::trim(Str);
			++i;
		}
	}

	inline std::string CopyToNewString(std::string::const_iterator begin, std::string::const_iterator end)
	{
		assert(end >= begin);

		std::string str;
		str.resize(end - begin);
		std::copy(begin, end, str.begin());
		return str;
	}

	inline std::string CopyToNewString(const std::string& Value, size_t begin, size_t end)
	{
		assert(end >= begin);
		return CopyToNewString(Value.begin() + begin, Value.begin() + end);
	}

	template <typename F>
	inline void StoiPassAllEceptionsWrapper(F func)
	{
		try
		{
			func();
		}
		catch (const std::invalid_argument&)
		{
		}
		catch (const std::out_of_range&)
		{
		}
	}

#ifndef DISABLE_ICU
	inline bool StringIsUpper(const std::string& Str)
	{
		icu::UnicodeString usLineStr = icu::UnicodeString::fromUTF8(icu::StringPiece(Str.c_str()));
		icu::UnicodeString usLineStrOrig = icu::UnicodeString::fromUTF8(icu::StringPiece(Str.c_str()));
		return (usLineStr.toUpper() == usLineStrOrig);
	}
#endif // DISABLE_ICU

	// Тестировалось только разбиение на две строки
	inline void SplitStringBySubstring(
		std::vector<std::string>& Splitted,
		const std::string& Str,
		const std::string& Del)
	{
		size_t ppos = 0;
		size_t pos;

		while ((pos = Str.find(Del, ppos)) != std::string::npos)
		{
			assert(pos >= ppos);
			Splitted.push_back(Str.substr(ppos, pos - ppos));
			ppos = pos + Del.size();
			assert(ppos <= Str.size());
		}
		Splitted.push_back(Str.substr(ppos, pos));
	}

};

