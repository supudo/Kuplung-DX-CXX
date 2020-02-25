#include "pch.h"
#include "CXXUtils.h"

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