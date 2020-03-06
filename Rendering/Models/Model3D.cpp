#include "pch.h"
#include "Model3D.h"
#include "Common\DirectXHelper.h"

using namespace Kuplung_DX::Rendering;
using namespace DirectX;
using namespace Windows::Foundation;

Models::Model3D::Model3D(const std::shared_ptr<DX::DeviceResources>& deviceResources) :
	m_deviceResources(deviceResources),
	m_loadingComplete(false),
	m_degreesPerSecond(45),
	m_indexCount(0),
	m_tracking(false) {
}

void Models::Model3D::InitModel3D(const Kuplung_DX::Models::MeshModel& model) {
	this->MeshModel = model;
	this->MatrixModel = XMMatrixIdentity();
	CreateDeviceDependentResources();
	this->InitProperties();
}

void Models::Model3D::InitProperties() {
	this->PositionX = std::make_unique<ObjectCoordinate>(false, 0.0f);
	this->PositionY = std::make_unique<ObjectCoordinate>(false, 0.0f);
	this->PositionZ = std::make_unique<ObjectCoordinate>(false, 0.0f);

	this->ScaleX = std::make_unique<ObjectCoordinate>(false, 1.0f);
	this->ScaleY = std::make_unique<ObjectCoordinate>(false, 1.0f);
	this->ScaleZ = std::make_unique<ObjectCoordinate>(false, 1.0f);

	this->RotateX = std::make_unique<ObjectCoordinate>(false, 0.0f);
	this->RotateY = std::make_unique<ObjectCoordinate>(false, 0.0f);
	this->RotateZ = std::make_unique<ObjectCoordinate>(false, 0.0f);
}

void Models::Model3D::CreateDeviceDependentResources() {
	//Size outputSize = this->m_deviceResources->GetOutputSize();
	//float aspectRatio = outputSize.Width / outputSize.Height;
	//float fovAngleY = 70.0f * XM_PI / 180.0f;

	//if (aspectRatio < 1.0f)
	//	fovAngleY *= 2.0f;

	//XMMATRIX perspectiveMatrix = XMMatrixPerspectiveFovRH(
	//	fovAngleY,
	//	aspectRatio,
	//	0.01f,
	//	100.0f
	//);
	//XMFLOAT4X4 orientation = m_deviceResources->GetOrientationTransform3D();
	//XMMATRIX orientationMatrix = XMLoadFloat4x4(&orientation);

	//XMStoreFloat4x4(
	//	&this->m_constantBufferData.projection,
	//	XMMatrixTranspose(perspectiveMatrix * orientationMatrix)
	//);

	//static const XMVECTORF32 eye = { 0.0f, 0.7f, 1.5f, 0.0f };
	//static const XMVECTORF32 at = { 0.0f, -0.1f, 0.0f, 0.0f };
	//static const XMVECTORF32 up = { 0.0f, 1.0f, 0.0f, 0.0f };

	//XMStoreFloat4x4(&this->m_constantBufferData.view, XMMatrixTranspose(XMMatrixLookAtRH(eye, at, up)));

	// Load shaders asynchronously.
	auto loadVSTask = DX::ReadDataAsync(L"RenderingSimpleVertexShader.cso");
	auto loadPSTask = DX::ReadDataAsync(L"RenderingSimplePixelShader.cso");

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
		static const XMFLOAT3 randomColors[] = {
			XMFLOAT3(0.0f, 0.0f, 0.0f),
			XMFLOAT3(0.0f, 0.0f, 1.0f),
			XMFLOAT3(0.0f, 1.0f, 0.0f),
			XMFLOAT3(0.0f, 1.0f, 1.0f),
			XMFLOAT3(1.0f, 0.0f, 0.0f),
			XMFLOAT3(1.0f, 0.0f, 1.0f),
			XMFLOAT3(1.0f, 1.0f, 0.0f),
			XMFLOAT3(1.0f, 1.0f, 1.0f),
		};

		static std::vector<VertexPositionColor> modelData;
		for (int i = 0; i < (int)this->MeshModel.vertices.size(); i++) {
			XMFLOAT3 v = this->MeshModel.vertices[i];
			XMFLOAT3 c = this->MeshModel.ModelMaterial.DiffuseColor;
			int ri = rand() % 8;
			c = randomColors[ri];
			modelData.push_back({ v, c });
		}

		D3D11_SUBRESOURCE_DATA vertexBufferData = { 0 };
		vertexBufferData.pSysMem = modelData.data();
		vertexBufferData.SysMemPitch = 0;
		vertexBufferData.SysMemSlicePitch = 0;
		uint32 s = sizeof(VertexPositionColor) * static_cast<UINT>(modelData.size());
		CD3D11_BUFFER_DESC vertexBufferDesc(s, D3D11_BIND_VERTEX_BUFFER);
		DX::ThrowIfFailed(
			m_deviceResources->GetD3DDevice()->CreateBuffer(
				&vertexBufferDesc,
				&vertexBufferData,
				&m_vertexBuffer
			)
		);

		static std::vector<unsigned int> modelIndices;
		for each (unsigned int i in this->MeshModel.indices) {
			modelIndices.push_back(i);
		}

		m_indexCount = this->MeshModel.countIndices;

		D3D11_SUBRESOURCE_DATA indexBufferData = { 0 };
		indexBufferData.pSysMem = modelIndices.data();
		indexBufferData.SysMemPitch = 0;
		indexBufferData.SysMemSlicePitch = 0;
		CD3D11_BUFFER_DESC indexBufferDesc(sizeof(unsigned int) * static_cast<UINT>(modelIndices.size()), D3D11_BIND_INDEX_BUFFER);
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
		this->m_loadingComplete = true;
		});
}

void Models::Model3D::ReleaseDeviceDependentResources() {
	this->m_loadingComplete = false;
	this->m_vertexShader.Reset();
	this->m_inputLayout.Reset();
	this->m_pixelShader.Reset();
	this->m_constantBuffer.Reset();
	this->m_vertexBuffer.Reset();
	this->m_indexBuffer.Reset();
}

void Models::Model3D::Render(const DirectX::XMFLOAT4X4 matrixProjection, const DirectX::XMFLOAT4X4 matrixCamera) {
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

void Models::Model3D::Update(DX::StepTimer const& timer) {
	if (!this->m_tracking) {
		float radiansPerSecond = XMConvertToRadians(this->m_degreesPerSecond);
		double totalRotation = timer.GetTotalSeconds() * radiansPerSecond;
		float radians = static_cast<float>(fmod(totalRotation, XM_2PI));
		this->Rotate(radians);
	}
}

void Models::Model3D::Rotate(float radians) {
	XMStoreFloat4x4(&this->m_constantBufferData.model, XMMatrixTranspose(XMMatrixRotationY(radians)));
}

void Models::Model3D::StartTracking() {
	this->m_tracking = true;
}

void Models::Model3D::TrackingUpdate(float positionX) {
	if (this->m_tracking) {
		float radians = XM_2PI * 2.0f * positionX / this->m_deviceResources->GetOutputSize().Width;
		this->Rotate(radians);
	}
}

void Models::Model3D::StopTracking() {
	this->m_tracking = false;
}