#include "pch.h"
#include "MathUtils.h"

void Kuplung_DX::Utilities::MathUtils::PrettyPrintMatrix4x4(const DirectX::XMFLOAT4X4 matrix) {
	std::string output("");
	for (int i = 0; i < 4; i++) {
		output += "[ ";
		for (int j = 0; j < 4; j++) {
			output += std::to_string(matrix(i, j));
			if (j < 3)
				output += ", ";
		}
		output += " ]\r\n";
	}
	Kuplung_DX::App::LogError(CXXUtils::StringToPlatformString(output));
}