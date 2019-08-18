// Reading ASCII, UTF-8, UTF-16LE, UTF-16BE with auto BOM detection using C++11 on Windows platform
// Code tested on Microsoft Visual Studio 2013 on Windows 7
// Part of the code is referencing http://cfc.kizzx2.com/index.php/reading-a-unicode-utf16-file-in-windows-c/

#include <cstdio>
#include <string>
#include <fstream>
#include <sstream>
#include <locale>
#include <codecvt>
#include <iostream>

#define TEXT_FILE_PATH      "D:\\test.txt"
#define ENCODING_ASCII      0
#define ENCODING_UTF8       1
#define ENCODING_UTF16LE    2
#define ENCODING_UTF16BE    3

std::stringstream readFile(std::string path)
{
	//std::string result;
	std::ifstream ifs(path.c_str()/*, std::ios::binary*/);
	std::stringstream ss;
	int encoding = ENCODING_ASCII;

	if (!ifs.is_open()) {
		// Unable to read file
		//result.clear();
		return ss;
	}
	else if (ifs.eof()) {
		//result.clear();
	}
	else {
		int ch1 = ifs.get();
		int ch2 = ifs.get();
		if (ch1 == 0xff && ch2 == 0xfe) {
			// The file contains UTF-16LE BOM
			encoding = ENCODING_UTF16LE;
		}
		else if (ch1 == 0xfe && ch2 == 0xff) {
			// The file contains UTF-16BE BOM
			encoding = ENCODING_UTF16BE;
		}
		else {
			int ch3 = ifs.get();
			if (ch1 == 0xef && ch2 == 0xbb && ch3 == 0xbf) {
				// The file contains UTF-8 BOM
				encoding = ENCODING_UTF8;
			}
			else {
				// The file does not have BOM
				encoding = ENCODING_ASCII;
				ifs.seekg(0);
			}
		}
	}
	ss << ifs.rdbuf();
	if (encoding == ENCODING_UTF16LE) {
		/*std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> utfconv;
		result = utfconv.to_bytes(std::wstring((wchar_t *)ss.str().c_str()));*/
		std::cerr << "Not implemented\n";
		std::terminate();
	}
	else if (encoding == ENCODING_UTF16BE) {
		/*std::string src = ss.str();
		std::string dst = src;
		// Using Windows API
		_swab(&src[0u], &dst[0u], src.size() + 1);
		std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> utfconv;
		result = utfconv.to_bytes(std::wstring((wchar_t *)dst.c_str()));*/
		std::cerr << "Not implemented\n";
		std::terminate();
	}
	else if (encoding == ENCODING_UTF8) {
		//result = ss.str();
	}
	else {
		//result = ss.str();
	}
	return ss;
}

