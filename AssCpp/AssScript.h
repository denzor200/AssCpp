#pragma once

#include "AssTime.h"
#include <bitset>

#include <boost\optional.hpp>

#include "AssDialogString.h"

namespace ASS {
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
		AssEventType	Type = AssEventType::UNKNOWN; // not initialized
		int				Layer;
		AssTime			Start;
		AssTime			End;
		std::string		Style;
		std::string		Name;
		uint32_t		MarginL;
		uint32_t		MarginR;
		uint32_t		MarginV;
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
	class AssSectionScriptInfo
	{
	public:
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
	private:
		ScriptType m_ScriptType = ScriptType::UNKNOWN; // not initialized
		boost::optional<Collisions> m_Collisions;
		boost::optional<uint32_t>	m_PlayResX;
		boost::optional<uint32_t>	m_PlayResY;
		boost::optional<uint32_t>	m_PlayDepth;
		boost::optional<double>		m_Timer; // Должно содержать 4 знака после запятой
		boost::optional<WrapStyle>	m_WrapStyle;

	public:
		AssSectionScriptInfo() = default;
		
		ScriptType GetScriptType() const;
		void SetScriptType(ScriptType Value);

		Collisions GetCollisions() const;
		void SetCollisions(Collisions Value);

		uint32_t GetPlayResX() const;
		void SetPlayResX(uint32_t Value);

		uint32_t GetPlayResY() const;
		void SetPlayResY(uint32_t Value);

		uint32_t GetPlayDepth() const;
		void SetPlayDepth(uint32_t Value);

		double GetTimer() const;
		void SetTimer(double Value);

		WrapStyle GetWrapStyle() const;
		void SetWrapStyle(WrapStyle Value);

	public:

		// Необязательные строки, не влияющие на работоспособность скрипта
		std::string Title;
		std::string OriginalScript;
		std::string OriginalTranslation;
		std::string OriginalEditing;
		std::string OriginalTiming;
		std::string SynchPoint;
		std::string ScriptUpdatedBy;
		std::string UpdateDetails;

	private:
		friend class AssScript;
		void Read(std::istream& Stream);
		void Write(std::ostream& Stream) const;
	};

	class AssSectionV4Styles
	{
	public:
		using SubSection = std::unordered_map<std::string, AssStyle>;
		using RefSubSection = SubSection & ;
		using CRefSubSection = const SubSection &;

	private:
		SubSection			m_Styles;
	public:
		RefSubSection		AllStyles();
		CRefSubSection		AllStyles() const;

	private:
		friend class AssScript;
		void Read(std::istream& Stream);
		void Write(std::ostream& Stream) const;
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

	private:
		friend class AssScript;
		void Read(std::istream& Stream);
		void Write(std::ostream& Stream) const;
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
		//AssScript() = default;
		AssScript(const std::string& FileName);
		//AssScript(std::stringstream& Stream);
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

		void Parse(std::istream& Stream);
		std::string Print() const;

		void Read(const std::string& FileName);
		void Write(const std::string& FileName) const;

	private:
		void ParseTo(AssImpl& Impl, std::istream& Stream);
		void Write(std::ostream& Stream) const;
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
