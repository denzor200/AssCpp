#pragma once

namespace EncodingUtils
{
	class InvalidEncoding : public std::exception
	{
	public:
		InvalidEncoding() : std::exception("Supports only utf-8 with BOM")
		{
		}
	};

	std::istream& PrepareStream(std::istream& Stream);
	std::ostream& PrepareStream(std::ostream& Stream);
};

