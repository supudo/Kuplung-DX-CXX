#include "pch.h"
#include "CXXUtils.h"

#include <locale>
#include <codecvt>
#include <string>

int32 Kuplung_DX::Utilities::CXXUtils::ConvertPlatformStringToInt32(Platform::String^ str) {
	std::wstring wsstr(str->Data());
	return std::stoi(wsstr);
}

Platform::String^ Kuplung_DX::Utilities::CXXUtils::ConvertInt32ToPlatformString(int num) {
    std::string s_str = std::to_string(num);
    std::wstring wid_str = std::wstring(s_str.begin(), s_str.end());
    const wchar_t* w_char = wid_str.c_str();
    return ref new Platform::String(w_char);
}

std::string Kuplung_DX::Utilities::CXXUtils::PlatformStringToString(Platform::String^ ms) {
	std::wstring w_str(ms->Begin());
	return std::string(w_str.begin(), w_str.end());
}

Platform::String^ Kuplung_DX::Utilities::CXXUtils::StringToPlatformString(const std::string& input) {
	std::wstring w_str = std::wstring(input.begin(), input.end());
	const wchar_t* w_chars = w_str.c_str();
	return (ref new Platform::String(w_chars));
}

std::wstring Kuplung_DX::Utilities::CXXUtils::StringToWString(std::string val) {
	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
	std::wstring wide = converter.from_bytes(val);
	return wide;
}

std::string Kuplung_DX::Utilities::CXXUtils::WStringToString(std::wstring val) {
	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
	std::string narrow = converter.to_bytes(val);
	return narrow;
}
