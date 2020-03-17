#include "pch.h"
#include "SampleLines.h"
#include <DX\DirectXHelper.h>

using namespace Kuplung_DX::Sample;

using namespace DirectX;
using namespace Windows::Foundation;

SampleLines::SampleLines(const std::shared_ptr<Kuplung_DX::DX::DeviceResources>& deviceResources) : m_loadingComplete(false), m_degreesPerSecond(45), m_indexCount(0), m_tracking(false), m_deviceResources(deviceResources) {
	this->CreateDeviceDependentResources();
	this->CreateWindowSizeDependentResources();
}

void SampleLines::CreateWindowSizeDependentResources() {
	Size outputSize = this->m_deviceResources->GetOutputSize();
	float aspectRatio = outputSize.Width / outputSize.Height;
	float fovAngleY = 70.0f * XM_PI / 180.0f;

	if (aspectRatio < 1.0f)
		fovAngleY *= 2.0f;

	XMMATRIX perspectiveMatrix = XMMatrixPerspectiveFovRH(fovAngleY, aspectRatio, 0.01f, 100.0f);
	XMFLOAT4X4 orientation = this->m_deviceResources->GetOrientationTransform3D();
	XMMATRIX orientationMatrix = XMLoadFloat4x4(&orientation);

	XMStoreFloat4x4(&this->m_constantBufferData.projection, XMMatrixTranspose(perspectiveMatrix * orientationMatrix));
	Kuplung_DX::App::LogError(L"SAMPLE: ");
	Kuplung_DX::Utilities::MathUtils::PrettyPrintMatrix4x4(this->m_constantBufferData.projection);

	static const XMVECTORF32 eye = { 0.0f, 0.7f, 1.5f, 0.0f };
	static const XMVECTORF32 at = { 0.0f, -0.1f, 0.0f, 0.0f };
	static const XMVECTORF32 up = { 0.0f, 1.0f, 0.0f, 0.0f };

	XMStoreFloat4x4(&this->m_constantBufferData.view, XMMatrixTranspose(XMMatrixLookAtRH(eye, at, up)));
}

void SampleLines::Update(Kuplung_DX::DX::StepTimer const& timer) {
	if (!this->m_tracking) {
		float radiansPerSecond = XMConvertToRadians(this->m_degreesPerSecond);
		double totalRotation = timer.GetTotalSeconds() * radiansPerSecond;
		float radians = static_cast<float>(fmod(totalRotation, XM_2PI));
		this->Rotate(radians);
	}
}

void SampleLines::Rotate(float radians) {
	XMStoreFloat4x4(&this->m_constantBufferData.model, XMMatrixTranspose(XMMatrixRotationY(radians)));
}

void SampleLines::StartTracking() {
	this->m_tracking = true;
}

void SampleLines::TrackingUpdate(float positionX) {
	if (this->m_tracking) {
		float radians = XM_2PI * 2.0f * positionX / this->m_deviceResources->GetOutputSize().Width;
		this->Rotate(radians);
	}
}

void SampleLines::StopTracking() {
	this->m_tracking = false;
}

void SampleLines::Render(bool useKuplungMatrix, const DirectX::XMFLOAT4X4 matrixProjection, const DirectX::XMFLOAT4X4 matrixCamera) {
	if (!this->m_loadingComplete)
		return;

	if (useKuplungMatrix) {
		this->m_constantBufferData.projection = matrixProjection;
		this->m_constantBufferData.view = matrixCamera;
	}

	//XMMATRIX x = XMLoadFloat4x4(&this->m_constantBufferData.projection) * XMLoadFloat4x4(&this->m_constantBufferData.view) * XMLoadFloat4x4(&this->m_constantBufferData.model);
	//Kuplung_DX::App::LogError(L"-------------------------------------------------------");
	//if (useKuplungMatrix)
	//	Kuplung_DX::App::LogError(L"KUPLUNG");
	//else
	//	Kuplung_DX::App::LogError(L"SAMPLE");
	//Kuplung_DX::App::LogError(L"Projection: ");
	//Kuplung_DX::Utilities::MathUtils::PrettyPrintMatrix4x4(m_constantBufferData.projection);
	//Kuplung_DX::App::LogError(L"View: ");
	//Kuplung_DX::Utilities::MathUtils::PrettyPrintMatrix4x4(m_constantBufferData.view);
	//Kuplung_DX::App::LogError(L"Model: ");
	//Kuplung_DX::Utilities::MathUtils::PrettyPrintMatrix4x4(m_constantBufferData.model);
	//Kuplung_DX::App::LogError(L"MVP: ");
	//Kuplung_DX::Utilities::MathUtils::PrettyPrintMatrixXM(x);

	auto context = this->m_deviceResources->GetD3DDeviceContext();

	context->UpdateSubresource1(this->m_constantBuffer.Get(), 0, NULL, &this->m_constantBufferData, 0, 0, 0);
	UINT stride = sizeof(VertexPositionColor);
	UINT offset = 0;
	context->IASetVertexBuffers(0, 1, this->m_vertexBuffer.GetAddressOf(), &stride, &offset);
	context->IASetIndexBuffer(this->m_indexBuffer.Get(), DXGI_FORMAT_R16_UINT, /* Each index is one 16-bit unsigned integer (short). */ 0);
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	context->IASetInputLayout(this->m_inputLayout.Get());
	context->VSSetShader(this->m_vertexShader.Get(), nullptr, 0);
	context->VSSetConstantBuffers1(0, 1, this->m_constantBuffer.GetAddressOf(), nullptr, nullptr);
	context->PSSetShader(this->m_pixelShader.Get(), nullptr, 0);
	context->DrawIndexed(this->m_indexCount, 0, 0);
}

void SampleLines::CreateDeviceDependentResources() {
	auto loadVSTask = Kuplung_DX::DX::ReadDataAsync(L"SampleVertexShader.cso");
	auto loadPSTask = Kuplung_DX::DX::ReadDataAsync(L"SamplePixelShader.cso");

	auto createVSTask = loadVSTask.then([this](const std::vector<byte>& fileData) {
		Kuplung_DX::DX::ThrowIfFailed(this->m_deviceResources->GetD3DDevice()->CreateVertexShader(&fileData[0], fileData.size(), nullptr, &this->m_vertexShader));
		static const D3D11_INPUT_ELEMENT_DESC vertexDesc [] = {
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};
		Kuplung_DX::DX::ThrowIfFailed(this->m_deviceResources->GetD3DDevice()->CreateInputLayout(vertexDesc, ARRAYSIZE(vertexDesc), &fileData[0], fileData.size(), &this->m_inputLayout));
	});

	auto createPSTask = loadPSTask.then([this](const std::vector<byte>& fileData) {
		Kuplung_DX::DX::ThrowIfFailed(this->m_deviceResources->GetD3DDevice()->CreatePixelShader(&fileData[0], fileData.size(), nullptr, &this->m_pixelShader));
		CD3D11_BUFFER_DESC constantBufferDesc(sizeof(ModelViewProjectionConstantBuffer) , D3D11_BIND_CONSTANT_BUFFER);
		Kuplung_DX::DX::ThrowIfFailed(this->m_deviceResources->GetD3DDevice()->CreateBuffer(&constantBufferDesc, nullptr, &this->m_constantBuffer));
	});

	auto createCubeTask = (createPSTask && createVSTask).then([this] () {
		static const VertexPositionColor cubeVertices[] =  {
			/*
			{XMFLOAT3(-0.5f, -0.5f, -0.5f), XMFLOAT3(0.0f, 0.0f, 0.0f)},
			{XMFLOAT3(-0.5f, -0.5f,  0.5f), XMFLOAT3(0.0f, 0.0f, 1.0f)},
			{XMFLOAT3(-0.5f,  0.5f, -0.5f), XMFLOAT3(0.0f, 1.0f, 0.0f)},
			{XMFLOAT3(-0.5f,  0.5f,  0.5f), XMFLOAT3(0.0f, 1.0f, 1.0f)},
			{XMFLOAT3( 0.5f, -0.5f, -0.5f), XMFLOAT3(1.0f, 0.0f, 0.0f)},
			{XMFLOAT3( 0.5f, -0.5f,  0.5f), XMFLOAT3(1.0f, 0.0f, 1.0f)},
			{XMFLOAT3( 0.5f,  0.5f, -0.5f), XMFLOAT3(1.0f, 1.0f, 0.0f)},
			{XMFLOAT3( 0.5f,  0.5f,  0.5f), XMFLOAT3(1.0f, 1.0f, 1.0f)},
			*/

			{ XMFLOAT3(-15.000000, -15.000000, 0.000000), XMFLOAT3(1.0f, 1.0f, 1.0f)},
			{ XMFLOAT3(15.000000, -15.000000, 0.000000), XMFLOAT3(1.0f, 1.0f, 1.0f)},
			{ XMFLOAT3(-15.000000, -14.000000, 0.000000), XMFLOAT3(1.0f, 1.0f, 1.0f)},
			{ XMFLOAT3(15.000000, -14.000000, 0.000000), XMFLOAT3(1.0f, 1.0f, 1.0f)},
			{ XMFLOAT3(-15.000000, -13.000000, 0.000000), XMFLOAT3(1.0f, 1.0f, 1.0f)},
			{ XMFLOAT3(15.000000, -13.000000, 0.000000), XMFLOAT3(1.0f, 1.0f, 1.0f)},
			{ XMFLOAT3(-15.000000, -12.000000, 0.000000), XMFLOAT3(1.0f, 1.0f, 1.0f)},
			{ XMFLOAT3(15.000000, -12.000000, 0.000000), XMFLOAT3(1.0f, 1.0f, 1.0f)},
			{ XMFLOAT3(-15.000000, -11.000000, 0.000000), XMFLOAT3(1.0f, 1.0f, 1.0f)},
			{ XMFLOAT3(15.000000, -11.000000, 0.000000), XMFLOAT3(1.0f, 1.0f, 1.0f)},
			{ XMFLOAT3(-15.000000, -10.000000, 0.000000), XMFLOAT3(1.0f, 1.0f, 1.0f)},
			{ XMFLOAT3(15.000000, -10.000000, 0.000000), XMFLOAT3(1.0f, 1.0f, 1.0f)},
			{ XMFLOAT3(-15.000000, -9.000000, 0.000000), XMFLOAT3(1.0f, 1.0f, 1.0f)},
			{ XMFLOAT3(15.000000, -9.000000, 0.000000), XMFLOAT3(1.0f, 1.0f, 1.0f)},
			{ XMFLOAT3(-15.000000, -8.000000, 0.000000), XMFLOAT3(1.0f, 1.0f, 1.0f)},
			{ XMFLOAT3(15.000000, -8.000000, 0.000000), XMFLOAT3(1.0f, 1.0f, 1.0f)},
			{ XMFLOAT3(-15.000000, -7.000000, 0.000000), XMFLOAT3(1.0f, 1.0f, 1.0f)},
			{ XMFLOAT3(15.000000, -7.000000, 0.000000), XMFLOAT3(1.0f, 1.0f, 1.0f)},
			{ XMFLOAT3(-15.000000, -6.000000, 0.000000), XMFLOAT3(1.0f, 1.0f, 1.0f)},
			{ XMFLOAT3(15.000000, -6.000000, 0.000000), XMFLOAT3(1.0f, 1.0f, 1.0f)},
			{ XMFLOAT3(-15.000000, -5.000000, 0.000000), XMFLOAT3(1.0f, 1.0f, 1.0f)},
			{ XMFLOAT3(15.000000, -5.000000, 0.000000), XMFLOAT3(1.0f, 1.0f, 1.0f)},
			{ XMFLOAT3(-15.000000, -4.000000, 0.000000), XMFLOAT3(1.0f, 1.0f, 1.0f)},
			{ XMFLOAT3(15.000000, -4.000000, 0.000000), XMFLOAT3(1.0f, 1.0f, 1.0f)},
			{ XMFLOAT3(-15.000000, -3.000000, 0.000000), XMFLOAT3(1.0f, 1.0f, 1.0f)},
			{ XMFLOAT3(15.000000, -3.000000, 0.000000), XMFLOAT3(1.0f, 1.0f, 1.0f)},
			{ XMFLOAT3(-15.000000, -2.000000, 0.000000), XMFLOAT3(1.0f, 1.0f, 1.0f)},
			{ XMFLOAT3(15.000000, -2.000000, 0.000000), XMFLOAT3(1.0f, 1.0f, 1.0f)},
			{ XMFLOAT3(-15.000000, -1.000000, 0.000000), XMFLOAT3(1.0f, 1.0f, 1.0f)},
			{ XMFLOAT3(15.000000, -1.000000, 0.000000), XMFLOAT3(1.0f, 1.0f, 1.0f)},
			{ XMFLOAT3(-15.000000, 0.000000, 0.000000), XMFLOAT3(1.0f, 1.0f, 1.0f)},
			{ XMFLOAT3(15.000000, 0.000000, 0.000000), XMFLOAT3(1.0f, 1.0f, 1.0f)},
			{ XMFLOAT3(-15.000000, 1.000000, 0.000000), XMFLOAT3(1.0f, 1.0f, 1.0f)},
			{ XMFLOAT3(15.000000, 1.000000, 0.000000), XMFLOAT3(1.0f, 1.0f, 1.0f)},
			{ XMFLOAT3(-15.000000, 2.000000, 0.000000), XMFLOAT3(1.0f, 1.0f, 1.0f)},
			{ XMFLOAT3(15.000000, 2.000000, 0.000000), XMFLOAT3(1.0f, 1.0f, 1.0f)},
			{ XMFLOAT3(-15.000000, 3.000000, 0.000000), XMFLOAT3(1.0f, 1.0f, 1.0f)},
			{ XMFLOAT3(15.000000, 3.000000, 0.000000), XMFLOAT3(1.0f, 1.0f, 1.0f)},
			{ XMFLOAT3(-15.000000, 4.000000, 0.000000), XMFLOAT3(1.0f, 1.0f, 1.0f)},
			{ XMFLOAT3(15.000000, 4.000000, 0.000000), XMFLOAT3(1.0f, 1.0f, 1.0f)},
			{ XMFLOAT3(-15.000000, 5.000000, 0.000000), XMFLOAT3(1.0f, 1.0f, 1.0f)},
			{ XMFLOAT3(15.000000, 5.000000, 0.000000), XMFLOAT3(1.0f, 1.0f, 1.0f)},
			{ XMFLOAT3(-15.000000, 6.000000, 0.000000), XMFLOAT3(1.0f, 1.0f, 1.0f)},
			{ XMFLOAT3(15.000000, 6.000000, 0.000000), XMFLOAT3(1.0f, 1.0f, 1.0f)},
			{ XMFLOAT3(-15.000000, 7.000000, 0.000000), XMFLOAT3(1.0f, 1.0f, 1.0f)},
			{ XMFLOAT3(15.000000, 7.000000, 0.000000), XMFLOAT3(1.0f, 1.0f, 1.0f)},
			{ XMFLOAT3(-15.000000, 8.000000, 0.000000), XMFLOAT3(1.0f, 1.0f, 1.0f)},
			{ XMFLOAT3(15.000000, 8.000000, 0.000000), XMFLOAT3(1.0f, 1.0f, 1.0f)},
			{ XMFLOAT3(-15.000000, 9.000000, 0.000000), XMFLOAT3(1.0f, 1.0f, 1.0f)},
			{ XMFLOAT3(15.000000, 9.000000, 0.000000), XMFLOAT3(1.0f, 1.0f, 1.0f)},
			{ XMFLOAT3(-15.000000, 10.000000, 0.000000), XMFLOAT3(1.0f, 1.0f, 1.0f)},
			{ XMFLOAT3(15.000000, 10.000000, 0.000000), XMFLOAT3(1.0f, 1.0f, 1.0f)},
			{ XMFLOAT3(-15.000000, 11.000000, 0.000000), XMFLOAT3(1.0f, 1.0f, 1.0f)},
			{ XMFLOAT3(15.000000, 11.000000, 0.000000), XMFLOAT3(1.0f, 1.0f, 1.0f)},
			{ XMFLOAT3(-15.000000, 12.000000, 0.000000), XMFLOAT3(1.0f, 1.0f, 1.0f)},
			{ XMFLOAT3(15.000000, 12.000000, 0.000000), XMFLOAT3(1.0f, 1.0f, 1.0f)},
			{ XMFLOAT3(-15.000000, 13.000000, 0.000000), XMFLOAT3(1.0f, 1.0f, 1.0f)},
			{ XMFLOAT3(15.000000, 13.000000, 0.000000), XMFLOAT3(1.0f, 1.0f, 1.0f)},
			{ XMFLOAT3(-15.000000, 14.000000, 0.000000), XMFLOAT3(1.0f, 1.0f, 1.0f)},
			{ XMFLOAT3(15.000000, 14.000000, 0.000000), XMFLOAT3(1.0f, 1.0f, 1.0f)},
			{ XMFLOAT3(-15.000000, 15.000000, 0.000000), XMFLOAT3(1.0f, 1.0f, 1.0f)},
			{ XMFLOAT3(15.000000, 15.000000, 0.000000), XMFLOAT3(1.0f, 1.0f, 1.0f)},
			{ XMFLOAT3(-15.000000, -15.000000, 0.000000), XMFLOAT3(1.0f, 1.0f, 1.0f)},
			{ XMFLOAT3(-15.000000, 15.000000, 0.000000), XMFLOAT3(1.0f, 1.0f, 1.0f)},
			{ XMFLOAT3(-14.000000, -15.000000, 0.000000), XMFLOAT3(1.0f, 1.0f, 1.0f)},
			{ XMFLOAT3(-14.000000, 15.000000, 0.000000), XMFLOAT3(1.0f, 1.0f, 1.0f)},
			{ XMFLOAT3(-13.000000, -15.000000, 0.000000), XMFLOAT3(1.0f, 1.0f, 1.0f)},
			{ XMFLOAT3(-13.000000, 15.000000, 0.000000), XMFLOAT3(1.0f, 1.0f, 1.0f)},
			{ XMFLOAT3(-12.000000, -15.000000, 0.000000), XMFLOAT3(1.0f, 1.0f, 1.0f)},
			{ XMFLOAT3(-12.000000, 15.000000, 0.000000), XMFLOAT3(1.0f, 1.0f, 1.0f)},
			{ XMFLOAT3(-11.000000, -15.000000, 0.000000), XMFLOAT3(1.0f, 1.0f, 1.0f)},
			{ XMFLOAT3(-11.000000, 15.000000, 0.000000), XMFLOAT3(1.0f, 1.0f, 1.0f)},
			{ XMFLOAT3(-10.000000, -15.000000, 0.000000), XMFLOAT3(1.0f, 1.0f, 1.0f)},
			{ XMFLOAT3(-10.000000, 15.000000, 0.000000), XMFLOAT3(1.0f, 1.0f, 1.0f)},
			{ XMFLOAT3(-9.000000, -15.000000, 0.000000), XMFLOAT3(1.0f, 1.0f, 1.0f)},
			{ XMFLOAT3(-9.000000, 15.000000, 0.000000), XMFLOAT3(1.0f, 1.0f, 1.0f)},
			{ XMFLOAT3(-8.000000, -15.000000, 0.000000), XMFLOAT3(1.0f, 1.0f, 1.0f)},
			{ XMFLOAT3(-8.000000, 15.000000, 0.000000), XMFLOAT3(1.0f, 1.0f, 1.0f)},
			{ XMFLOAT3(-7.000000, -15.000000, 0.000000), XMFLOAT3(1.0f, 1.0f, 1.0f)},
			{ XMFLOAT3(-7.000000, 15.000000, 0.000000), XMFLOAT3(1.0f, 1.0f, 1.0f)},
			{ XMFLOAT3(-6.000000, -15.000000, 0.000000), XMFLOAT3(1.0f, 1.0f, 1.0f)},
			{ XMFLOAT3(-6.000000, 15.000000, 0.000000), XMFLOAT3(1.0f, 1.0f, 1.0f)},
			{ XMFLOAT3(-5.000000, -15.000000, 0.000000), XMFLOAT3(1.0f, 1.0f, 1.0f)},
			{ XMFLOAT3(-5.000000, 15.000000, 0.000000), XMFLOAT3(1.0f, 1.0f, 1.0f)},
			{ XMFLOAT3(-4.000000, -15.000000, 0.000000), XMFLOAT3(1.0f, 1.0f, 1.0f)},
			{ XMFLOAT3(-4.000000, 15.000000, 0.000000), XMFLOAT3(1.0f, 1.0f, 1.0f)},
			{ XMFLOAT3(-3.000000, -15.000000, 0.000000), XMFLOAT3(1.0f, 1.0f, 1.0f)},
			{ XMFLOAT3(-3.000000, 15.000000, 0.000000), XMFLOAT3(1.0f, 1.0f, 1.0f)},
			{ XMFLOAT3(-2.000000, -15.000000, 0.000000), XMFLOAT3(1.0f, 1.0f, 1.0f)},
			{ XMFLOAT3(-2.000000, 15.000000, 0.000000), XMFLOAT3(1.0f, 1.0f, 1.0f)},
			{ XMFLOAT3(-1.000000, -15.000000, 0.000000), XMFLOAT3(1.0f, 1.0f, 1.0f)},
			{ XMFLOAT3(-1.000000, 15.000000, 0.000000), XMFLOAT3(1.0f, 1.0f, 1.0f)},
			{ XMFLOAT3(0.000000, 15.000000, 0.000000), XMFLOAT3(1.0f, 1.0f, 1.0f)},
			{ XMFLOAT3(0.000000, -15.000000, 0.000000), XMFLOAT3(1.0f, 1.0f, 1.0f)},
			{ XMFLOAT3(1.000000, -15.000000, 0.000000), XMFLOAT3(1.0f, 1.0f, 1.0f)},
			{ XMFLOAT3(1.000000, 15.000000, 0.000000), XMFLOAT3(1.0f, 1.0f, 1.0f)},
			{ XMFLOAT3(2.000000, -15.000000, 0.000000), XMFLOAT3(1.0f, 1.0f, 1.0f)},
			{ XMFLOAT3(2.000000, 15.000000, 0.000000), XMFLOAT3(1.0f, 1.0f, 1.0f)},
			{ XMFLOAT3(3.000000, -15.000000, 0.000000), XMFLOAT3(1.0f, 1.0f, 1.0f)},
			{ XMFLOAT3(3.000000, 15.000000, 0.000000), XMFLOAT3(1.0f, 1.0f, 1.0f)},
			{ XMFLOAT3(4.000000, -15.000000, 0.000000), XMFLOAT3(1.0f, 1.0f, 1.0f)},
			{ XMFLOAT3(4.000000, 15.000000, 0.000000), XMFLOAT3(1.0f, 1.0f, 1.0f)},
			{ XMFLOAT3(5.000000, -15.000000, 0.000000), XMFLOAT3(1.0f, 1.0f, 1.0f)},
			{ XMFLOAT3(5.000000, 15.000000, 0.000000), XMFLOAT3(1.0f, 1.0f, 1.0f)},
			{ XMFLOAT3(6.000000, -15.000000, 0.000000), XMFLOAT3(1.0f, 1.0f, 1.0f)},
			{ XMFLOAT3(6.000000, 15.000000, 0.000000), XMFLOAT3(1.0f, 1.0f, 1.0f)},
			{ XMFLOAT3(7.000000, -15.000000, 0.000000), XMFLOAT3(1.0f, 1.0f, 1.0f)},
			{ XMFLOAT3(7.000000, 15.000000, 0.000000), XMFLOAT3(1.0f, 1.0f, 1.0f)},
			{ XMFLOAT3(8.000000, -15.000000, 0.000000), XMFLOAT3(1.0f, 1.0f, 1.0f)},
			{ XMFLOAT3(8.000000, 15.000000, 0.000000), XMFLOAT3(1.0f, 1.0f, 1.0f)},
			{ XMFLOAT3(9.000000, -15.000000, 0.000000), XMFLOAT3(1.0f, 1.0f, 1.0f)},
			{ XMFLOAT3(9.000000, 15.000000, 0.000000), XMFLOAT3(1.0f, 1.0f, 1.0f)},
			{ XMFLOAT3(10.000000, -15.000000, 0.000000), XMFLOAT3(1.0f, 1.0f, 1.0f)},
			{ XMFLOAT3(10.000000, 15.000000, 0.000000), XMFLOAT3(1.0f, 1.0f, 1.0f)},
			{ XMFLOAT3(11.000000, -15.000000, 0.000000), XMFLOAT3(1.0f, 1.0f, 1.0f)},
			{ XMFLOAT3(11.000000, 15.000000, 0.000000), XMFLOAT3(1.0f, 1.0f, 1.0f)},
			{ XMFLOAT3(12.000000, -15.000000, 0.000000), XMFLOAT3(1.0f, 1.0f, 1.0f)},
			{ XMFLOAT3(12.000000, 15.000000, 0.000000), XMFLOAT3(1.0f, 1.0f, 1.0f)},
			{ XMFLOAT3(13.000000, -15.000000, 0.000000), XMFLOAT3(1.0f, 1.0f, 1.0f)},
			{ XMFLOAT3(13.000000, 15.000000, 0.000000), XMFLOAT3(1.0f, 1.0f, 1.0f)},
			{ XMFLOAT3(14.000000, -15.000000, 0.000000), XMFLOAT3(1.0f, 1.0f, 1.0f)},
			{ XMFLOAT3(14.000000, 15.000000, 0.000000), XMFLOAT3(1.0f, 1.0f, 1.0f)},
			{ XMFLOAT3(15.000000, -15.000000, 0.000000), XMFLOAT3(1.0f, 1.0f, 1.0f)},
			{ XMFLOAT3(15.000000, 15.000000, 0.000000), XMFLOAT3(1.0f, 1.0f, 1.0f)},
			{ XMFLOAT3(0.000000, 0.000000, 15.000000), XMFLOAT3(1.0f, 1.0f, 1.0f)},
			{ XMFLOAT3(0.000000, 0.000000, -15.000000), XMFLOAT3(1.0f, 1.0f, 1.0f)},
			{ XMFLOAT3(0.000000, 0.000000, 15.000000), XMFLOAT3(1.0f, 1.0f, 1.0f)},
			{ XMFLOAT3(0.000000, 0.000000, -15.000000), XMFLOAT3(1.0f, 1.0f, 1.0f)}
		};

		D3D11_SUBRESOURCE_DATA vertexBufferData = {0};
		vertexBufferData.pSysMem = cubeVertices;
		vertexBufferData.SysMemPitch = 0;
		vertexBufferData.SysMemSlicePitch = 0;
		CD3D11_BUFFER_DESC vertexBufferDesc(sizeof(cubeVertices), D3D11_BIND_VERTEX_BUFFER);
		Kuplung_DX::DX::ThrowIfFailed(this->m_deviceResources->GetD3DDevice()->CreateBuffer(&vertexBufferDesc, &vertexBufferData, &this->m_vertexBuffer));

		static const unsigned short cubeIndices [] = {
			/*
			0,2,1, // -x
			1,2,3,

			4,5,6, // +x
			5,7,6,

			0,1,5, // -y
			0,5,4,

			2,6,7, // +y
			2,7,3,

			0,4,6, // -z
			0,6,2,

			1,3,7, // +z
			1,7,5,
			*/
			0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111, 112, 113, 114, 115, 116, 117, 118, 119, 120, 121, 122, 123, 124, 125, 126, 127
		};

		this->m_indexCount = ARRAYSIZE(cubeIndices);

		D3D11_SUBRESOURCE_DATA indexBufferData = {0};
		indexBufferData.pSysMem = cubeIndices;
		indexBufferData.SysMemPitch = 0;
		indexBufferData.SysMemSlicePitch = 0;
		CD3D11_BUFFER_DESC indexBufferDesc(sizeof(cubeIndices), D3D11_BIND_INDEX_BUFFER);
		Kuplung_DX::DX::ThrowIfFailed(this->m_deviceResources->GetD3DDevice()->CreateBuffer(&indexBufferDesc, &indexBufferData, &this->m_indexBuffer));
	});

	createCubeTask.then([this] () {
		this->m_loadingComplete = true;
	});
}

void SampleLines::ReleaseDeviceDependentResources() {
	this->m_loadingComplete = false;
	this->m_vertexShader.Reset();
	this->m_inputLayout.Reset();
	this->m_pixelShader.Reset();
	this->m_constantBuffer.Reset();
	this->m_vertexBuffer.Reset();
	this->m_indexBuffer.Reset();
}
