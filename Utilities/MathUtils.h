#pragma once

#include <sstream>

namespace Kuplung_DX
{
	namespace Utilities
	{
		public ref class MathUtils sealed
		{
		internal:
			static void PrettyPrintMatrix4x4(const DirectX::XMFLOAT4X4 matrix);
		};
	}
}

