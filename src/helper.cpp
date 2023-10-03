#include <locale>  // std::wstring_convert<T>
#include <codecvt> // std::codecvt_utf8_utf16<wchar_t>

#include "helper.hpp"

WString UTF8_To_WideString(const String& utf8_str) {
	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
	return converter.from_bytes(utf8_str);
}

String WideString_To_UTF8(const WString& wide_str) {
	std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
	return converter.to_bytes(wide_str);
}

