﻿#include "pch.h"
#include "ObjectsManager.h"
#include <DX\DirectXHelper.h>

using namespace Kuplung_DX::Rendering;
using namespace Windows::Foundation;

ObjectsManager::ObjectsManager(const std::shared_ptr<Kuplung_DX::DX::DeviceResources>& deviceResources) : m_deviceResources(deviceResources) {
	XMStoreFloat4x4(&this->MatrixProjection, DirectX::XMMatrixIdentity());

	this->CompCamera = std::make_unique<Kuplung_DX::Models::Camera>();
	this->CompCamera->InitProperties();

	this->CompGrid = std::make_unique<Kuplung_DX::Models::GUI::Grid>(this->m_deviceResources);
	this->CompGrid->InitBuffers(Kuplung_DX::App::GridSize, Kuplung_DX::App::GridUnitSize);
}

ObjectsManager::~ObjectsManager() {
	this->CompCamera.reset();
	this->CompGrid.reset();
}

void ObjectsManager::Render() {
	Size outputSize = m_deviceResources->GetOutputSize();
	float aspectRatio = outputSize.Width / outputSize.Height;
	float fovAngleY = Kuplung_DX::App::Setting_FOV * XM_PI / 180.0f;
	if (aspectRatio < 1.0f)
		fovAngleY *= 2.0f;

	XMMATRIX perspectiveMatrix = XMMatrixPerspectiveFovRH(fovAngleY, aspectRatio, Kuplung_DX::App::Setting_PlaneClose, Kuplung_DX::App::Setting_PlaneFar);
	XMFLOAT4X4 orientation = this->m_deviceResources->GetOrientationTransform3D();
	XMMATRIX orientationMatrix = XMLoadFloat4x4(&orientation);
	XMStoreFloat4x4(&this->MatrixProjection, XMMatrixTranspose(perspectiveMatrix * orientationMatrix));
	//Kuplung_DX::App::LogError(L"KUPLUNG: ");
	//Kuplung_DX::Utilities::MathUtils::PrettyPrintMatrix4x4(this->MatrixProjection);

	this->CompCamera->Render();
	
	if (Kuplung_DX::App::ShowGrid)
		this->CompGrid->Render(this->MatrixProjection, this->CompCamera->MatrixCamera);
}
