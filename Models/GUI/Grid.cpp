#include "pch.h"
#include "Grid.h"
#include "Common\DirectXHelper.h"
#include <Models\ShaderStructures.h>

using namespace Kuplung_DX::Models::GUI;
using namespace DirectX;
using namespace Windows::Foundation;

Grid::Grid(const std::shared_ptr<DX::DeviceResources>& deviceResources) : m_deviceResources(deviceResources), gridSizeVertex(10) {
    this->gridSize = 10;
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
}

void Grid::InitProperties() {
    this->showGrid = true;

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

void Grid::InitBuffers(const int& gridSize, const float& unitSize) {
	this->gridSize = gridSize % 2 == 0 ? gridSize + 1 : gridSize;
	this->gridSizeVertex = this->gridSize;

	if (this->gridSizeVertex % 2 == 0)
		this->gridSizeVertex += 1;

	this->dataVertices.clear();
	this->dataColors.clear();
	this->dataIndices.clear();

	uint32 indiceCounter = 0;
	const float perLines = (float)std::ceil(this->gridSizeVertex / 2);

	// +
	for (float z = perLines; z > 0; z--) {
		this->dataVertices.push_back(XMFLOAT3(-perLines * unitSize, 0.0, -z * unitSize));
		this->dataVertices.push_back(XMFLOAT3(perLines * unitSize, 0.0, -z * unitSize));
		this->dataColors.push_back(XMFLOAT3(0.7f, 0.7f, 0.7f));
		this->dataColors.push_back(XMFLOAT3(0.7f, 0.7f, 0.7f));
		this->dataIndices.push_back(indiceCounter++);
		this->dataIndices.push_back(indiceCounter++);
	}

	// X
	this->dataVertices.push_back(XMFLOAT3(-perLines * unitSize, 0.0, 0.0));
	this->dataVertices.push_back(XMFLOAT3(perLines * unitSize, 0.0, 0.0));
	this->dataColors.push_back(XMFLOAT3(1.0, 0.0, 0.0));
	this->dataColors.push_back(XMFLOAT3(1.0, 0.0, 0.0));
	this->dataIndices.push_back(indiceCounter++);
	this->dataIndices.push_back(indiceCounter++);
	// X

	for (float z = 1.0; z <= perLines; z++) {
		this->dataVertices.push_back(XMFLOAT3(-perLines * unitSize, 0.0, z * unitSize));
		this->dataVertices.push_back(XMFLOAT3(perLines * unitSize, 0.0, z * unitSize));
		this->dataColors.push_back(XMFLOAT3(0.7f, 0.7f, 0.7f));
		this->dataColors.push_back(XMFLOAT3(0.7f, 0.7f, 0.7f));
		this->dataIndices.push_back(indiceCounter++);
		this->dataIndices.push_back(indiceCounter++);
	}

	// -
	for (float x = perLines; x > 0; x--) {
		this->dataVertices.push_back(XMFLOAT3(-x * unitSize, 0.0, -perLines * unitSize));
		this->dataVertices.push_back(XMFLOAT3(-x * unitSize, 0.0, perLines * unitSize));
		this->dataColors.push_back(XMFLOAT3(0.7f, 0.7f, 0.7f));
		this->dataColors.push_back(XMFLOAT3(0.7f, 0.7f, 0.7f));
		this->dataIndices.push_back(indiceCounter++);
		this->dataIndices.push_back(indiceCounter++);
	}

	// Z
	this->dataVertices.push_back(XMFLOAT3(0.0, 0.0, perLines * unitSize));
	this->dataVertices.push_back(XMFLOAT3(0.0, 0.0, -perLines * unitSize));
	this->dataColors.push_back(XMFLOAT3(0.0, 0.0, 1.0));
	this->dataColors.push_back(XMFLOAT3(0.0, 0.0, 1.0));
	this->dataIndices.push_back(indiceCounter++);
	this->dataIndices.push_back(indiceCounter++);
	// Z

	for (float x = 1.0; x <= perLines; x++) {
		this->dataVertices.push_back(XMFLOAT3(x * unitSize, 0.0, -perLines * unitSize));
		this->dataVertices.push_back(XMFLOAT3(x * unitSize, 0.0, perLines * unitSize));
		this->dataColors.push_back(XMFLOAT3(0.7f, 0.7f, 0.7f));
		this->dataColors.push_back(XMFLOAT3(0.7f, 0.7f, 0.7f));
		this->dataIndices.push_back(indiceCounter++);
		this->dataIndices.push_back(indiceCounter++);
	}

	// Y
	this->dataVertices.push_back(XMFLOAT3(0.0, perLines * unitSize, 0.0));
	this->dataVertices.push_back(XMFLOAT3(0.0, -perLines * unitSize, 0.0));
	this->dataColors.push_back(XMFLOAT3(0.0, 1.0, 0.0));
	this->dataColors.push_back(XMFLOAT3(0.0, 1.0, 0.0));
	this->dataIndices.push_back(indiceCounter++);
	this->dataIndices.push_back(indiceCounter++);

	this->dataVertices.push_back(XMFLOAT3(0.0, perLines * unitSize, 0.0));
	this->dataVertices.push_back(XMFLOAT3(0.0, -perLines * unitSize, 0.0));
	this->dataColors.push_back(XMFLOAT3(0.0, 1.0, 0.0));
	this->dataColors.push_back(XMFLOAT3(0.0, 1.0, 0.0));
	this->dataIndices.push_back(indiceCounter++);
	this->dataIndices.push_back(indiceCounter++);

	CreateDeviceDependentResources();
	CreateWindowSizeDependentResources();
}

void Grid::Render(const XMFLOAT4X4& matrixProjection, const XMFLOAT4X4& matrixCamera) {
	if (!this->m_loadingComplete)
		return;

	this->m_constantBufferData.projection = matrixProjection;
	this->m_constantBufferData.view = matrixCamera;

	this->MatrixModel = XMMatrixIdentity();
	this->MatrixModel = XMMatrixScaling(this->ScaleX->point, this->ScaleY->point, this->ScaleZ->point);
	this->MatrixModel = XMMatrixTranslation(0, 0, 0);
	this->MatrixModel = XMMatrixRotationX(this->RotateX->point);
	this->MatrixModel = XMMatrixRotationY(this->RotateY->point);
	this->MatrixModel = XMMatrixRotationZ(this->RotateZ->point);
	this->MatrixModel = XMMatrixTranslation(0, 0, 0);
	this->MatrixModel = XMMatrixTranslation(this->PositionX->point, this->PositionY->point, this->PositionZ->point);

	//XMMATRIX mvpMatrix = XMMatrixMultiply(XMLoadFloat4x4(&matrixProjection), XMLoadFloat4x4(&matrixCamera));
	//mvpMatrix = XMMatrixMultiply(mvpMatrix, this->MatrixModel);
	XMStoreFloat4x4(&this->m_constantBufferData.model, this->MatrixModel);

	auto context = this->m_deviceResources->GetD3DDeviceContext();
	context->UpdateSubresource1(this->m_constantBuffer.Get(), 0, NULL, &this->m_constantBufferData, 0, 0, 0);
	UINT stride = sizeof(VertexPositionColor);
	UINT offset = 0;
	context->IASetVertexBuffers(0, 1, this->m_vertexBuffer.GetAddressOf(), &stride, &offset);
	context->IASetIndexBuffer(this->m_indexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0);
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	context->IASetInputLayout(this->m_inputLayout.Get());
	context->VSSetShader(this->m_vertexShader.Get(), nullptr, 0);
	context->VSSetConstantBuffers1(0, 1, this->m_constantBuffer.GetAddressOf(), nullptr, nullptr);
	context->PSSetShader(this->m_pixelShader.Get(), nullptr, 0);
	context->DrawIndexed(this->m_indexCount, 0, 0);
}

void Grid::CreateWindowSizeDependentResources() {
	Size outputSize = this->m_deviceResources->GetOutputSize();
	float aspectRatio = outputSize.Width / outputSize.Height;
	float fovAngleY = 70.0f * XM_PI / 180.0f;

	if (aspectRatio < 1.0f)
		fovAngleY *= 2.0f;

	XMMATRIX perspectiveMatrix = XMMatrixPerspectiveFovRH(fovAngleY, aspectRatio, 0.01f, 100.0f);
	XMFLOAT4X4 orientation = this->m_deviceResources->GetOrientationTransform3D();
	XMMATRIX orientationMatrix = XMLoadFloat4x4(&orientation);

	XMStoreFloat4x4(&this->m_constantBufferData.projection, XMMatrixTranspose(perspectiveMatrix * orientationMatrix));

	static const XMVECTORF32 eye = { 0.0f, 0.7f, 1.5f, 0.0f };
	static const XMVECTORF32 at = { 0.0f, -0.1f, 0.0f, 0.0f };
	static const XMVECTORF32 up = { 0.0f, 1.0f, 0.0f, 0.0f };

	XMStoreFloat4x4(&this->m_constantBufferData.view, XMMatrixTranspose(XMMatrixLookAtRH(eye, at, up)));
	XMStoreFloat4x4(&this->m_constantBufferData.model, XMMatrixTranspose(XMMatrixRotationY(45)));
}

void Grid::CreateDeviceDependentResources() {
	auto loadVSTask = DX::ReadDataAsync(L"GridVertexShader.cso");
	auto loadPSTask = DX::ReadDataAsync(L"GridPixelShader.cso");

	// After the vertex shader file is loaded, create the shader and input layout.
	auto createVSTask = loadVSTask.then([this](const std::vector<byte>& fileData) {
		DX::ThrowIfFailed(
			m_deviceResources->GetD3DDevice()->CreateVertexShader(
				&fileData[0],
				fileData.size(),
				nullptr,
				&m_vertexShader
			)
		);

		static const D3D11_INPUT_ELEMENT_DESC vertexDesc[] = {
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};

		DX::ThrowIfFailed(
			m_deviceResources->GetD3DDevice()->CreateInputLayout(
				vertexDesc,
				ARRAYSIZE(vertexDesc),
				&fileData[0],
				fileData.size(),
				&m_inputLayout
			)
		);
		});

	// After the pixel shader file is loaded, create the shader and constant buffer.
	auto createPSTask = loadPSTask.then([this](const std::vector<byte>& fileData) {
		DX::ThrowIfFailed(
			m_deviceResources->GetD3DDevice()->CreatePixelShader(
				&fileData[0],
				fileData.size(),
				nullptr,
				&m_pixelShader
			)
		);

		CD3D11_BUFFER_DESC constantBufferDesc(sizeof(ModelViewProjectionConstantBuffer), D3D11_BIND_CONSTANT_BUFFER);
		DX::ThrowIfFailed(
			m_deviceResources->GetD3DDevice()->CreateBuffer(
				&constantBufferDesc,
				nullptr,
				&m_constantBuffer
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
			m_deviceResources->GetD3DDevice()->CreateBuffer(
				&vertexBufferDesc,
				&vertexBufferData,
				&m_vertexBuffer
			)
		);

		this->m_indexCount = (int)this->dataIndices.size();

		D3D11_SUBRESOURCE_DATA indexBufferData = { 0 };
		indexBufferData.pSysMem = this->dataIndices.data();
		indexBufferData.SysMemPitch = 0;
		indexBufferData.SysMemSlicePitch = 0;
		CD3D11_BUFFER_DESC indexBufferDesc(sizeof(unsigned int) * static_cast<UINT>(this->dataIndices.size()), D3D11_BIND_INDEX_BUFFER);
		DX::ThrowIfFailed(
			m_deviceResources->GetD3DDevice()->CreateBuffer(
				&indexBufferDesc,
				&indexBufferData,
				&m_indexBuffer
			)
		);
		});

	// Once the cube is loaded, the object is ready to be rendered.
	createCubeTask.then([this]() {
		m_loadingComplete = true;
		});
}

void Grid::ReleaseDeviceDependentResources() {
	m_loadingComplete = false;
	m_vertexShader.Reset();
	m_inputLayout.Reset();
	m_pixelShader.Reset();
	m_constantBuffer.Reset();
	m_vertexBuffer.Reset();
	m_indexBuffer.Reset();
}
