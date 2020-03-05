#include "pch.h"
#include "ObjectsManager.h"
#include "..\Common\DirectXHelper.h"

using namespace Kuplung_DX::Rendering;

ObjectsManager::ObjectsManager(const std::shared_ptr<DX::DeviceResources>& deviceResources) : m_deviceResources(deviceResources) {
	XMStoreFloat4x4(&this->MatrixProjection, DirectX::XMMatrixIdentity());
	XMStoreFloat4x4(&this->MatrixCamera, DirectX::XMMatrixIdentity());

	this->CompCamera = std::make_unique<Kuplung_DX::Models::Camera>();
	this->CompCamera->InitProperties();

	this->compGrid = std::make_unique<Kuplung_DX::Models::GUI::Grid>(this->m_deviceResources);
	this->compGrid->InitBuffers(Kuplung_DX::App::GridSize, Kuplung_DX::App::GridUnitSize);
}

ObjectsManager::~ObjectsManager() {
	this->compGrid.reset();
}

void ObjectsManager::Render() {
	XMMATRIX perspectiveMatrix = XMMatrixPerspectiveFovRH(
		Kuplung_DX::App::Setting_FOV,
		Kuplung_DX::App::Setting_RatioWidth / Kuplung_DX::App::Setting_RatioHeight,
		Kuplung_DX::App::Setting_PlaneClose,
		Kuplung_DX::App::Setting_PlaneFar
	);
	XMFLOAT4X4 orientation = this->m_deviceResources->GetOrientationTransform3D();
	XMMATRIX orientationMatrix = XMLoadFloat4x4(&orientation);
	XMStoreFloat4x4(&this->MatrixProjection, XMMatrixTranspose(perspectiveMatrix * orientationMatrix));
	XMStoreFloat4x4(&this->MatrixCamera, XMMatrixTranspose(XMMatrixLookAtRH(this->CompCamera->EyeSettings->View_Eye, this->CompCamera->EyeSettings->View_Center, this->CompCamera->EyeSettings->View_Up)));

	this->CompCamera->Render();
	
	if (Kuplung_DX::App::ShowGrid)
		this->compGrid->Render(this->MatrixProjection, this->MatrixCamera);
}
