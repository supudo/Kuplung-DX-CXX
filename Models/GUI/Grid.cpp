#include "pch.h"
#include "Grid.h"
#include <DX\DirectXHelper.h>
#include <Models\ShaderStructures.h>

using namespace Kuplung_DX;
using namespace Kuplung_DX::Models::GUI;
using namespace DirectX;
using namespace Windows::Foundation;

Grid::Grid(const std::shared_ptr<DX::DeviceResources>& deviceResources) : m_deviceResources(deviceResources) {
    this->InitProperties();
}

Grid::~Grid() {
    this->PositionX.reset();
    this->PositionY.reset();
    this->PositionZ.reset();

    this->ScaleX.reset();
    this->ScaleY.reset();
    this->ScaleZ.reset();

    this->RotateX.reset();
    this->RotateY.reset();
    this->RotateZ.reset();

	this->ReleaseDeviceDependentResources();
}

void Grid::InitProperties() {
    this->PositionX = std::make_unique<ObjectCoordinate>(false, 0.0f);
    this->PositionY = std::make_unique<ObjectCoordinate>(false, 0.0f);
    this->PositionZ = std::make_unique<ObjectCoordinate>(false, 0.0f);

    this->ScaleX = std::make_unique<ObjectCoordinate>(false, 1.0f);
    this->ScaleY = std::make_unique<ObjectCoordinate>(false, 1.0f);
    this->ScaleZ = std::make_unique<ObjectCoordinate>(false, 1.0f);

    this->RotateX = std::make_unique<ObjectCoordinate>(false, 0.0f);
    this->RotateY = std::make_unique<ObjectCoordinate>(false, 0.0f);
    this->RotateZ = std::make_unique<ObjectCoordinate>(false, 0.0f);

    this->MatrixModel = XMMatrixIdentity();
}

void Grid::InitBuffers(const int& gridSize, const int& unitSize) {
	this->gridSize = gridSize;
	this->gridUnitSize = unitSize;

	this->dataVertices.clear();
	this->dataColors.clear();
	this->dataIndices.clear();

	this->m_indexCount = 0;
	const float perLines = (float)std::ceil(this->gridSize / 2);

	// +
	for (float z = perLines; z > 0; z--) {
		this->dataVertices.push_back(XMFLOAT3(-perLines * unitSize, 0.0, -z * unitSize));
		this->dataVertices.push_back(XMFLOAT3(perLines * unitSize, 0.0, -z * unitSize));
		this->dataColors.push_back(XMFLOAT3(0.7f, 0.7f, 0.7f));
		this->dataColors.push_back(XMFLOAT3(0.7f, 0.7f, 0.7f));
		this->dataIndices.push_back(this->m_indexCount++);
		this->dataIndices.push_back(this->m_indexCount++);
	}

	// X
	this->dataVertices.push_back(XMFLOAT3(-perLines * unitSize, 0.0, 0.0));
	this->dataVertices.push_back(XMFLOAT3(perLines * unitSize, 0.0, 0.0));
	this->dataColors.push_back(XMFLOAT3(1.0, 0.0, 0.0));
	this->dataColors.push_back(XMFLOAT3(1.0, 0.0, 0.0));
	this->dataIndices.push_back(this->m_indexCount++);
	this->dataIndices.push_back(this->m_indexCount++);
	// X

	for (float z = 1.0; z <= perLines; z++) {
		this->dataVertices.push_back(XMFLOAT3(-perLines * unitSize, 0.0, z * unitSize));
		this->dataVertices.push_back(XMFLOAT3(perLines * unitSize, 0.0, z * unitSize));
		this->dataColors.push_back(XMFLOAT3(0.7f, 0.7f, 0.7f));
		this->dataColors.push_back(XMFLOAT3(0.7f, 0.7f, 0.7f));
		this->dataIndices.push_back(this->m_indexCount++);
		this->dataIndices.push_back(this->m_indexCount++);
	}

	// -
	for (float x = perLines; x > 0; x--) {
		this->dataVertices.push_back(XMFLOAT3(-x * unitSize, 0.0, -perLines * unitSize));
		this->dataVertices.push_back(XMFLOAT3(-x * unitSize, 0.0, perLines * unitSize));
		this->dataColors.push_back(XMFLOAT3(0.7f, 0.7f, 0.7f));
		this->dataColors.push_back(XMFLOAT3(0.7f, 0.7f, 0.7f));
		this->dataIndices.push_back(this->m_indexCount++);
		this->dataIndices.push_back(this->m_indexCount++);
	}

	// Z
	this->dataVertices.push_back(XMFLOAT3(0.0, 0.0, perLines * unitSize));
	this->dataVertices.push_back(XMFLOAT3(0.0, 0.0, -perLines * unitSize));
	this->dataColors.push_back(XMFLOAT3(0.0, 0.0, 1.0));
	this->dataColors.push_back(XMFLOAT3(0.0, 0.0, 1.0));
	this->dataIndices.push_back(this->m_indexCount++);
	this->dataIndices.push_back(this->m_indexCount++);
	// Z

	for (float x = 1.0; x <= perLines; x++) {
		this->dataVertices.push_back(XMFLOAT3(x * unitSize, 0.0, -perLines * unitSize));
		this->dataVertices.push_back(XMFLOAT3(x * unitSize, 0.0, perLines * unitSize));
		this->dataColors.push_back(XMFLOAT3(0.7f, 0.7f, 0.7f));
		this->dataColors.push_back(XMFLOAT3(0.7f, 0.7f, 0.7f));
		this->dataIndices.push_back(this->m_indexCount++);
		this->dataIndices.push_back(this->m_indexCount++);
	}

	// Y
	this->dataVertices.push_back(XMFLOAT3(0.0, perLines * unitSize, 0.0));
	this->dataVertices.push_back(XMFLOAT3(0.0, -perLines * unitSize, 0.0));
	this->dataColors.push_back(XMFLOAT3(0.0, 1.0, 0.0));
	this->dataColors.push_back(XMFLOAT3(0.0, 1.0, 0.0));
	this->dataIndices.push_back(this->m_indexCount++);
	this->dataIndices.push_back(this->m_indexCount++);

	this->dataVertices.push_back(XMFLOAT3(0.0, perLines * unitSize, 0.0));
	this->dataVertices.push_back(XMFLOAT3(0.0, -perLines * unitSize, 0.0));
	this->dataColors.push_back(XMFLOAT3(0.0, 1.0, 0.0));
	this->dataColors.push_back(XMFLOAT3(0.0, 1.0, 0.0));
	this->dataIndices.push_back(this->m_indexCount++);
	this->dataIndices.push_back(this->m_indexCount++);

	this->CreateDeviceDependentResources();
}

void Grid::Render(const XMFLOAT4X4& matrixProjection, const XMFLOAT4X4& matrixCamera) {
	if (this->gridSize != Kuplung_DX::App::GridSize) {
		this->m_loadingComplete = false;
		this->InitBuffers(Kuplung_DX::App::GridSize, Kuplung_DX::App::GridUnitSize);
	}
	if (!this->m_loadingComplete)
		return;

	this->m_constantBufferData.projection = matrixProjection;
	this->m_constantBufferData.view = matrixCamera;

	this->MatrixModel = XMMatrixIdentity();
	this->MatrixModel = XMMatrixScaling(this->ScaleX->point, this->ScaleY->point, this->ScaleZ->point);
	this->MatrixModel = XMMatrixTranspose(XMMatrixRotationRollPitchYaw(XMConvertToRadians(this->RotateX->point), XMConvertToRadians(this->RotateY->point), XMConvertToRadians(this->RotateZ->point)));
	this->MatrixModel = XMMatrixTranslation(this->PositionX->point, this->PositionY->point, this->PositionZ->point);

	XMStoreFloat4x4(&this->m_constantBufferData.model, this->MatrixModel);

	auto context = this->m_deviceResources->GetD3DDeviceContext();
	context->UpdateSubresource1(this->m_constantBuffer.Get(), 0, NULL, &this->m_constantBufferData, 0, 0, 0);
	UINT stride = sizeof(VertexPositionColor);
	UINT offset = 0;
	context->IASetVertexBuffers(0, 1, this->m_vertexBuffer.GetAddressOf(), &stride, &offset);
	context->IASetIndexBuffer(this->m_indexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0);
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
	context->IASetInputLayout(this->m_inputLayout.Get());
	context->VSSetShader(this->m_vertexShader.Get(), nullptr, 0);
	context->VSSetConstantBuffers1(0, 1, this->m_constantBuffer.GetAddressOf(), nullptr, nullptr);
	context->PSSetShader(this->m_pixelShader.Get(), nullptr, 0);
	context->DrawIndexed(this->m_indexCount, 0, 0);
}

void Grid::CreateDeviceDependentResources() {
	auto loadVSTask = DX::ReadDataAsync(L"GridVertexShader.cso");
	auto loadPSTask = DX::ReadDataAsync(L"GridPixelShader.cso");

	// After the vertex shader file is loaded, create the shader and input layout.
	auto createVSTask = loadVSTask.then([this](const std::vector<byte>& fileData) {
		DX::ThrowIfFailed(
			this->m_deviceResources->GetD3DDevice()->CreateVertexShader(
				&fileData[0],
				fileData.size(),
				nullptr,
				&this->m_vertexShader
			)
		);

		static const D3D11_INPUT_ELEMENT_DESC vertexDesc[] = {
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};

		DX::ThrowIfFailed(
			this->m_deviceResources->GetD3DDevice()->CreateInputLayout(
				vertexDesc,
				ARRAYSIZE(vertexDesc),
				&fileData[0],
				fileData.size(),
				&this->m_inputLayout
			)
		);
		});

	// After the pixel shader file is loaded, create the shader and constant buffer.
	auto createPSTask = loadPSTask.then([this](const std::vector<byte>& fileData) {
		DX::ThrowIfFailed(
			this->m_deviceResources->GetD3DDevice()->CreatePixelShader(
				&fileData[0],
				fileData.size(),
				nullptr,
				&this->m_pixelShader
			)
		);

		CD3D11_BUFFER_DESC constantBufferDesc(sizeof(ModelViewProjectionConstantBuffer), D3D11_BIND_CONSTANT_BUFFER);
		DX::ThrowIfFailed(
			this->m_deviceResources->GetD3DDevice()->CreateBuffer(
				&constantBufferDesc,
				nullptr,
				&this->m_constantBuffer
			)
		);
		});

	// Once both shaders are loaded, create the mesh.
	auto createCubeTask = (createPSTask && createVSTask).then([this]() {
		static std::vector<VertexPositionColor> modelVertices;
		for (int i = 0; i < (int)this->dataVertices.size(); i++) {
			XMFLOAT3 v = this->dataVertices[i];
			XMFLOAT3 c = this->dataColors[i];
			modelVertices.push_back({ v, c });
		}

		D3D11_SUBRESOURCE_DATA vertexBufferData = { 0 };
		vertexBufferData.pSysMem = modelVertices.data();
		vertexBufferData.SysMemPitch = 0;
		vertexBufferData.SysMemSlicePitch = 0;
		uint32 s = sizeof(VertexPositionColor) * static_cast<UINT>(modelVertices.size());
		CD3D11_BUFFER_DESC vertexBufferDesc(s, D3D11_BIND_VERTEX_BUFFER);
		DX::ThrowIfFailed(
			this->m_deviceResources->GetD3DDevice()->CreateBuffer(
				&vertexBufferDesc,
				&vertexBufferData,
				&this->m_vertexBuffer
			)
		);

		this->m_indexCount = (int)this->dataIndices.size();

		D3D11_SUBRESOURCE_DATA indexBufferData = { 0 };
		indexBufferData.pSysMem = this->dataIndices.data();
		indexBufferData.SysMemPitch = 0;
		indexBufferData.SysMemSlicePitch = 0;
		CD3D11_BUFFER_DESC indexBufferDesc(sizeof(UINT) * static_cast<UINT>(this->dataIndices.size()), D3D11_BIND_INDEX_BUFFER);
		DX::ThrowIfFailed(
			this->m_deviceResources->GetD3DDevice()->CreateBuffer(
				&indexBufferDesc,
				&indexBufferData,
				&this->m_indexBuffer
			)
		);
		});

	// Once the cube is loaded, the object is ready to be rendered.
	createCubeTask.then([this]() {
		this->m_loadingComplete = true;
		});
}

void Grid::ReleaseDeviceDependentResources() {
	this->m_loadingComplete = false;
	this->m_vertexShader.Reset();
	this->m_inputLayout.Reset();
	this->m_pixelShader.Reset();
	this->m_constantBuffer.Reset();
	this->m_vertexBuffer.Reset();
	this->m_indexBuffer.Reset();
}
