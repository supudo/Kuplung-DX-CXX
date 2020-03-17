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
			static void PrettyPrintMatrixXM(const DirectX::XMMATRIX matrix);
			static void PrettyPrintDataArray(const std::vector<XMFLOAT3> data);
			static void PrettyPrintDataIndices(const std::vector<UINT> data);
		};
	}
}

