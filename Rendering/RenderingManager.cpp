#include "pch.h"
#include "RenderingManager.h"

using namespace DirectX;
using namespace Windows::Foundation;

Kuplung_DX::Rendering::RenderingManager::RenderingManager(const std::shared_ptr<DX::DeviceResources>& deviceResources) {
	this->renderingSimple = std::make_unique<Kuplung_DX::Rendering::RenderingSimple>(deviceResources);
}

void Kuplung_DX::Rendering::RenderingManager::CreateWindowSizeDependentResources() {
	this->renderingSimple->CreateWindowSizeDependentResources();
}

void Kuplung_DX::Rendering::RenderingManager::Update(DX::StepTimer const& timer) {
	switch (Kuplung_DX::App::RenderingMethod)
	{
	case 1:
		this->renderingSimple->Update(timer);
		break;
	default:
		break;
	}
}

void Kuplung_DX::Rendering::RenderingManager::Rotate(float radians) {
	switch (Kuplung_DX::App::RenderingMethod)
	{
	case 1:
		this->renderingSimple->Rotate(radians);
		break;
	default:
		break;
	}
}

void Kuplung_DX::Rendering::RenderingManager::StartTracking() {
	switch (Kuplung_DX::App::RenderingMethod)
	{
	case 1:
		this->renderingSimple->StartTracking();
		break;
	default:
		break;
	}
}

void Kuplung_DX::Rendering::RenderingManager::TrackingUpdate(float positionX) {
	switch (Kuplung_DX::App::RenderingMethod)
	{
	case 1:
		this->renderingSimple->TrackingUpdate(positionX);
		break;
	default:
		break;
	}
}

void Kuplung_DX::Rendering::RenderingManager::StopTracking() {
	switch (Kuplung_DX::App::RenderingMethod)
	{
	case 1:
		this->renderingSimple->StopTracking();
		break;
	default:
		break;
	}
}

void Kuplung_DX::Rendering::RenderingManager::Render(const std::vector<Kuplung_DX::Models::MeshModel>& models) {
	switch (Kuplung_DX::App::RenderingMethod)
	{
	case 1:
		this->renderingSimple->Render(models);
		break;
	default:
		break;
	}
}

void Kuplung_DX::Rendering::RenderingManager::CreateDeviceDependentResources() {
	this->renderingSimple->CreateDeviceDependentResources();
}

void Kuplung_DX::Rendering::RenderingManager::ReleaseDeviceDependentResources() {
	this->renderingSimple->ReleaseDeviceDependentResources();
}

bool Kuplung_DX::Rendering::RenderingManager::IsTracking() {
	switch (Kuplung_DX::App::RenderingMethod)
	{
	case 1:
		return this->renderingSimple->IsTracking();
		break;
	default:
		return false;
		break;
	}
}