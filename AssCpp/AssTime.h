#pragma once

namespace ASS
{

	class AssTime
	{
		uint32_t m_Hours = 0;
		uint32_t m_Minutes = 0;
		uint32_t m_Seconds = 0;
		uint32_t m_Miliseconds = 0;
	public:
		AssTime(uint32_t Miliseconds);
		AssTime(uint32_t Hours, uint32_t Minutes, uint32_t Seconds);
		AssTime(uint32_t Hours, uint32_t Minutes, uint32_t Seconds, uint32_t Miliseconds);
		AssTime() = default;

		uint32_t GetHours() const noexcept;

		uint32_t GetMinutes() const noexcept;

		uint32_t GetSeconds() const noexcept;

		uint32_t GetMiliseconds() const noexcept;

		void SetHours(uint32_t Hours);

		void SetMinutes(uint32_t Minutes);

		void SetSeconds(uint32_t Seconds);

		void SetMiliseconds(uint32_t Miliseconds);

		uint32_t TotalMiliseconds() const noexcept;

		struct WriteParams
		{
			bool WithMiliseconds = true;
			bool UseFloor = false; // TODO: implement UseFloor

			WriteParams() = default;
		};

		void Parse(const std::string& Value);
		std::string Print(WriteParams Params = WriteParams()) const;
	};
}

namespace ASS
{
	inline uint32_t AssTime::GetHours() const noexcept
	{
		return m_Hours;
	}

	inline uint32_t AssTime::GetMinutes() const noexcept
	{
		return m_Minutes;
	}

	inline uint32_t AssTime::GetSeconds() const noexcept
	{
		return m_Seconds;
	}

	inline uint32_t AssTime::GetMiliseconds() const noexcept
	{
		return m_Miliseconds;
	}
}
