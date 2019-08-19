#pragma once

#include <boost/date_time/posix_time/posix_time.hpp>
#include <bitset>

#include "AssDialogString.h"

namespace ASS {

	// Время в ass имеет свои нюансы
	// 1. Час должен содержать не более одной цифры
	// 2. Милисекунды имеют точность 1/100 секунды
	// Возможно по этим причинам стоит реализовать свой класс времени
	using AssTime = boost::posix_time::ptime;

	// TODO: некоторые стороковые поля возможно следует заменить ссылками на объекты
	struct AssEvent
	{
		enum class AssEventType
		{
			UNKNOWN = 0,
			Dialogue	= 1,
			Comment		= 2,
			Picture		= 3,
			Sound		= 4,
			Movie		= 5,
			Command		= 6
		};
		static const uint32_t EVENTS_COUNT = 6;
		AssEventType	Type = AssEventType::UNKNOWN; 
		bool			Marked = false;
		AssTime			Start;
		AssTime			End;
		std::string		Style;
		std::string		Name;
		uint32_t		MarginL = 0;
		uint32_t		MarginR = 0;
		uint32_t		MarginV = 0 ;
		std::string		Effect;
		std::string		Text;

		AssEvent() = default;
	};

	struct AssStyle
	{
		enum class BorderStyle
		{
			ONE,
			THREE
		};
		enum class Alignment
		{
			// По умолчанию вертикальное выравнивание идет по нижнему краю кадра.
			Left,
			Center,
			Right,

			LeftVUp,
			CenterVUp,
			RightVUp,

			LeftVCenter,
			CenterVCenter,
			RightVCenter,

			LeftVDown,
			CenterVDown,
			RightVDown
		};

		bool Initialized = false;
		std::string Name;
		std::string Fontname;
		double		Fontsize = 0;
		uint32_t	PrimaryColour = 0;
		uint32_t	SecondaryColour = 0;
		uint32_t	OutlineColor = 0;
		uint32_t	BackColour = 0;
		// TODO: Field 4-7
		bool		Bold = false;
		bool		Italic = false;
		bool		Underline = false;
		bool		StrikeOut = false;
		uint32_t	ScaleX = 100;
		uint32_t	ScaleY = 100;
		uint32_t	Spacing = 0;
		double		Angle = 0;
		BorderStyle BorderStyle = BorderStyle::ONE;
		uint32_t	Outline = 0;
		uint32_t	Shadow = 0;
		Alignment	Alignment = Alignment::Center;
		uint32_t	MarginL = 0;
		uint32_t	MarginR = 0;
		uint32_t	MarginV = 0;
		uint32_t	Encoding = 1;	// у нас пока поддерживается только utf-8
									// это поле можно игнорировать

		AssStyle() = default;
	};

	// Header
	struct AssSectionScriptInfo
	{
		enum class ScriptType
		{
			UNKNOWN = 0,
			V400plus // Пока поддерживаем только один тип
		};
		enum class Collisions
		{
			Normal,
			Reverse
		};
		enum class WrapStyle
		{
			ZERO,
			FIRST,
			SECOND,
			THIRD
		};

		// Строки, влияющие на работоспособность и правильность отображения субитров
		ScriptType ScriptType = ScriptType::UNKNOWN;
		Collisions Collisions = Collisions::Normal; // TODO: выясни значение по умолчанию
		uint32_t PlayResX = 0;
		uint32_t PlayResY = 0;
		uint32_t PlayDepth = 0;
		double Timer = 100.0000; // Должно содержать 4 знака после запятой
		WrapStyle WrapStyle = WrapStyle::ZERO; // TODO: выясни значение по умолчанию

		// Необязательные строки, не влияющие на работоспособность скрипта
		// TODO: нужно как-то отслежтивать факт наличия необязательной строки
		// Чтобы они потом не шли в выходной файл мусором
		std::string Title;
		std::string OriginalScript;
		std::string OriginalTranslation;
		std::string OriginalEditing;
		std::string OriginalTiming;
		std::string SynchPoint;
		std::string ScriptUpdatedBy;
		std::string UpdateDetails;

		AssSectionScriptInfo() = default;
	};

	class AssSectionV4Styles
	{
	public:
		using SubSection = std::unordered_map<std::string, AssStyle>;
		using RefSubSection = SubSection & ;
		using CRefSubSection = const SubSection &;

	private:

	public:
		RefSubSection		AllStyles();
		CRefSubSection		AllStyles() const;

	};

	class AssSectionEvents
	{
	public:
		using SubSection = std::vector<AssEvent>;
		using RefSubSection = SubSection & ;
		using CRefSubSection = const SubSection &;

	private:
		SubSection m_Events;
	public:
		RefSubSection		AllEvents();
		CRefSubSection		AllEvents() const;

		template <typename T>
		void EnumerateEvents(T Func, std::bitset<AssEvent::EVENTS_COUNT> Flags);

		template <typename T>
		void EnumerateEvents(T Func);

	};

	// Пока не поддерживается..
	class AssSectionFonts 
	{
	public:

	};

	// Пока не поддерживается..
	class AssSectionGraphics 
	{
	public:

	};

	// TODO: Подбери другой базовый класс. Не факт, что стоит наследоваться от std::exception
	class AssScriptException : public std::exception
	{
	protected:
		AssScriptException(const char* Msg) noexcept : std::exception(Msg)
		{
		}
	};

	// TODO: вывести в текст ошибок имена файлов
	class AssScriptIsNotTextFile : public AssScriptException
	{
	public:
		AssScriptIsNotTextFile() noexcept :
			AssScriptException("Can't parse ass script because it's not a text file")
		{
		}
	};

	class AssScriptCantFindScriptInfo : public AssScriptException
	{
	public:
		AssScriptCantFindScriptInfo() noexcept :
			AssScriptException("Can't find 'Script Info' section in ass script")
		{
		}
	};

	class AssScript
	{
	public:
		using RefAssSectionScriptInfo = AssSectionScriptInfo & ;
		using CRefAssSectionScriptInfo = const AssSectionScriptInfo&;

		using RefAssSectionV4Styles = AssSectionV4Styles & ;
		using CRefAssSectionV4Styles = const AssSectionV4Styles&;

		using RefAssSectionEvents = AssSectionEvents & ;
		using CRefAssSectionEvents = const AssSectionEvents&;

		using RefAssSectionFonts = AssSectionFonts & ;
		using CRefAssSectionFonts = const AssSectionFonts &;

		using RefAssSectionGraphics = AssSectionGraphics & ;
		using CRefAssSectionGraphics = const AssSectionGraphics&;

	private:
		struct AssImpl 
		{
			AssSectionScriptInfo ScriptInfo;
			AssSectionV4Styles V4Styles;
			AssSectionEvents Events;
			AssSectionFonts Fonts;
			AssSectionGraphics Graphics;

			AssImpl() = default;
		};
		AssImpl m_Impl;

	public:
		AssScript() = default;
		AssScript(const std::string& FileName);
		AssScript(std::stringstream& ss);
		~AssScript();

		RefAssSectionScriptInfo		ScriptInfo();
		CRefAssSectionScriptInfo	ScriptInfo() const;

		RefAssSectionV4Styles		V4Styles();
		CRefAssSectionV4Styles		V4Styles() const;

		RefAssSectionEvents			Events();
		CRefAssSectionEvents		Events() const;

		RefAssSectionFonts			Fonts();
		CRefAssSectionFonts			Fonts() const;

		RefAssSectionGraphics		Graphics();
		CRefAssSectionGraphics		Graphics() const;

		void Parse(std::stringstream& ss);
		std::string Print() const;

		void Read(const std::string& FileName);
		void Write(const std::string& FileName) const;

	private:
		void ParseTo(AssImpl& Impl, std::stringstream& ss);
	};
};


namespace ASS
{
	inline AssScript::RefAssSectionScriptInfo	AssScript::ScriptInfo()
	{
		return m_Impl.ScriptInfo;
	}
	inline AssScript::CRefAssSectionScriptInfo	AssScript::ScriptInfo() const
	{
		return m_Impl.ScriptInfo;
	}

	inline AssScript::RefAssSectionV4Styles		AssScript::V4Styles()
	{
		return m_Impl.V4Styles;
	}
	inline AssScript::CRefAssSectionV4Styles	AssScript::V4Styles() const
	{
		return m_Impl.V4Styles;
	}

	inline AssScript::RefAssSectionEvents		AssScript::Events()
	{
		return m_Impl.Events;
	}
	inline AssScript::CRefAssSectionEvents		AssScript::Events() const
	{
		return m_Impl.Events;
	}

	inline AssScript::RefAssSectionFonts		AssScript::Fonts()
	{
		return m_Impl.Fonts;
	}
	inline AssScript::CRefAssSectionFonts		AssScript::Fonts() const
	{
		return m_Impl.Fonts;
	}

	inline AssScript::RefAssSectionGraphics		AssScript::Graphics()
	{
		return m_Impl.Graphics;
	}
	inline AssScript::CRefAssSectionGraphics	AssScript::Graphics() const
	{
		return m_Impl.Graphics;
	}

	inline AssSectionEvents::RefSubSection		AssSectionEvents::AllEvents()
	{
		return m_Events;
	}
	inline AssSectionEvents::CRefSubSection		AssSectionEvents::AllEvents() const
	{
		return m_Events;
	}


	template <typename T>
	void AssSectionEvents::EnumerateEvents(T Func, std::bitset<AssEvent::EVENTS_COUNT> Flags)
	{
		for (auto Value : m_Events)
		{
			assert(Value.Type != AssEventType::UNKNOWN);
			if (Flags.test(Value.Type-1))
				Func(Value);
		}
	}

	template <typename T>
	void AssSectionEvents::EnumerateEvents(T Func)
	{
		EnumerateEvents(Func, std::bitset<AssEvent::EVENTS_COUNT>().set());
	}

};
