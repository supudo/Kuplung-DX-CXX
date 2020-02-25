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
		};
	}
}

