#pragma once

#include <sstream>

namespace Kuplung_DX
{
	namespace Utilities
	{
		public ref class CXXUtils sealed
		{
		public:
			static int32 ConvertPlatformStringToInt32(Platform::String^ str);
			static Platform::String^ ConvertInt32ToPlatformString(int num);

		internal:
			template<typename ... Args>
			static std::string StringFormat(const std::string& format, Args ... args) {
				size_t size = snprintf(nullptr, 0, format.c_str(), args ...) + 1;
				std::unique_ptr<char[]> buf(new char[size]);
				snprintf(buf.get(), size, format.c_str(), args ...);
				return std::string(buf.get(), buf.get() + size - 1);
			}

			template<typename ... Args>
			static Platform::String^ FormatStringToPlatformString(const std::string& format, Args ... args) {
				size_t size = snprintf(nullptr, 0, format.c_str(), args ...) + 1;
				std::unique_ptr<char[]> buf(new char[size]);
				snprintf(buf.get(), size, format.c_str(), args ...);
				std::string s = std::string(buf.get(), buf.get() + size - 1);
				return StringToPlatformString(s);
			}

			static Platform::String^ StringToPlatformString(const std::string& input);
			static std::string PlatformStringToString(Platform::String^ ms);
			static std::wstring StringToWString(std::string val);
			static std::string WStringToString(std::wstring val);
		};
	}
}

