#pragma once

#include <exception>

namespace ASS
{

	/*******************************
	* Root exception class
	********************************/
	// TODO: Подбери другой базовый класс. Не факт, что стоит наследоваться от std::exception
	class AssException : public std::exception
	{
	protected:
		AssException(const char* Msg) noexcept : std::exception(Msg)
		{
		}
	};

	/*******************************
	* Parsing
	********************************/
	// TODO: вывести в текст ошибок имена файлов
	class AssScriptIsNotTextFile : public AssException
	{
	public:
		AssScriptIsNotTextFile() noexcept :
			AssException("Can't parse ass script because it's not a text file")
		{
		}
	};

	class AssScriptCantFindScriptInfo : public AssException
	{
	public:
		AssScriptCantFindScriptInfo() noexcept :
			AssException("Can't find 'Script Info' section in ass script")
		{
		}
	};

	class AssScriptInvalidScriptType : public AssException
	{
	public:
		AssScriptInvalidScriptType() noexcept :
			AssException("The value of 'ScriptType' field is not expected. Supported only 'v4.00+' now")
		{
		}
	};


	/*******************************
	* Timing
	********************************/
	class AssTimeInvalidString : public AssException
	{
	public:
		// TODO: make error string
		AssTimeInvalidString() noexcept : AssException("")
		{
		}
	};

	class AssTimeInvalidValue : public AssException
	{
	public:
		enum class ValueType
		{
			Hours,
			Minutes,
			Seconds,
			Miliseconds
		};
		uint32_t	m_Value;
		ValueType	m_ValueType;
	public:
		// TODO: make message string
		AssTimeInvalidValue(ValueType VT, uint32_t V) noexcept :
			AssException(""),
			m_ValueType(VT),
			m_Value(V)
		{

		}
		uint32_t	GetValue() const { return m_Value; }
		ValueType	GetValueType() const { return m_ValueType; }
	};

};

