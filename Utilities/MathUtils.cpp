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

void Kuplung_DX::Utilities::MathUtils::PrettyPrintMatrixXM(const DirectX::XMMATRIX matrix) {
	DirectX::XMFLOAT4X4 mtx;
	XMStoreFloat4x4(&mtx, matrix);
	std::string output("");
	for (int i = 0; i < 4; i++) {
		output += "[ ";
		for (int j = 0; j < 4; j++) {
			output += std::to_string(mtx(i, j));
			if (j < 3)
				output += ", ";
		}
		output += " ]\r\n";
	}
	Kuplung_DX::App::LogError(CXXUtils::StringToPlatformString(output));
}

void Kuplung_DX::Utilities::MathUtils::PrettyPrintDataArray(const std::vector<XMFLOAT3> data) {
	std::string output("");
	for (int i = 0; i < data.size(); i++) {
		output += "{ XMFLOAT3(" + std::to_string(data.at(i).x) + ", " + std::to_string(data.at(i).y) + ", " + std::to_string(data.at(i).z) + "), XMFLOAT3(1.0f, 1.0f, 1.0f)} \r\n";
	}
	Kuplung_DX::App::LogError(CXXUtils::StringToPlatformString(output));
}

void Kuplung_DX::Utilities::MathUtils::PrettyPrintDataIndices(const std::vector<UINT> data) {
	std::string output("");
	for (int i = 0; i < data.size(); i++) {
		output += std::to_string(data.at(i)) + ", ";
	}
	Kuplung_DX::App::LogError(CXXUtils::StringToPlatformString(output));
}