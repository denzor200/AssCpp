#include "stdafx.h"
#include "EncodingUtils.h"

#define ENCODING_ASCII      0
#define ENCODING_UTF8       1
#define ENCODING_UTF16LE    2
#define ENCODING_UTF16BE    3


std::istream& EncodingUtils::PrepareStream(std::istream& Stream)
{
	int encoding;
	int ch1 = Stream.get();
	int ch2 = Stream.get();
	if (ch1 == 0xff && ch2 == 0xfe) {
		// The file contains UTF-16LE BOM
		encoding = ENCODING_UTF16LE;
	}
	else if (ch1 == 0xfe && ch2 == 0xff) {
		// The file contains UTF-16BE BOM
		encoding = ENCODING_UTF16BE;
	}
	else {
		int ch3 = Stream.get();
		if (ch1 == 0xef && ch2 == 0xbb && ch3 == 0xbf) {
			// The file contains UTF-8 BOM
			encoding = ENCODING_UTF8;
		}
		else {
			// The file does not have BOM
			encoding = ENCODING_ASCII;
			Stream.seekg(0);
		}
	}
	if (ENCODING_UTF8 != encoding)
	{
		throw InvalidEncoding();
	}
	return Stream;
}

std::ostream& EncodingUtils::PrepareStream(std::ostream& Stream)
{
	static const unsigned char bom[] = { 0xEF,0xBB,0xBF };
	Stream.write((char*)bom, sizeof(bom));
	return Stream;
}

