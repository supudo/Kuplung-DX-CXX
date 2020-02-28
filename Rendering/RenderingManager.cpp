#include "pch.h"
#include "RenderingManager.h"

using namespace DirectX;
using namespace Windows::Foundation;

Kuplung_DX::Rendering::RenderingManager::RenderingManager(const std::shared_ptr<DX::DeviceResources>& deviceResources) {
	this->renderingSimple = std::make_unique<Kuplung_DX::Rendering::RenderingSimple>();
}

void Kuplung_DX::Rendering::RenderingManager::Render(const std::vector<std::unique_ptr<Kuplung_DX::Rendering::Models::Model3D>>& models) {
	switch (Kuplung_DX::App::RenderingMethod)
	{
	case 1:
		this->renderingSimple->Render(models);
		break;
	default:
		break;
	}
}

void Kuplung_DX::Rendering::RenderingManager::Update(const std::vector<std::unique_ptr<Kuplung_DX::Rendering::Models::Model3D>>& models, DX::StepTimer const& timer) {
	switch (Kuplung_DX::App::RenderingMethod)
	{
	case 1:
		this->renderingSimple->Update(models, timer);
		break;
	default:
		break;
	}
}

void Kuplung_DX::Rendering::RenderingManager::Rotate(const std::vector<std::unique_ptr<Kuplung_DX::Rendering::Models::Model3D>>& models, float radians) {
	switch (Kuplung_DX::App::RenderingMethod)
	{
	case 1:
		this->renderingSimple->Rotate(models, radians);
		break;
	default:
		break;
	}
}

void Kuplung_DX::Rendering::RenderingManager::StartTracking(const std::vector<std::unique_ptr<Kuplung_DX::Rendering::Models::Model3D>>& models) {
	switch (Kuplung_DX::App::RenderingMethod)
	{
	case 1:
		this->renderingSimple->StartTracking(models);
		break;
	default:
		break;
	}
}

void Kuplung_DX::Rendering::RenderingManager::TrackingUpdate(const std::vector<std::unique_ptr<Kuplung_DX::Rendering::Models::Model3D>>& models, float positionX) {
	switch (Kuplung_DX::App::RenderingMethod)
	{
	case 1:
		this->renderingSimple->TrackingUpdate(models, positionX);
		break;
	default:
		break;
	}
}

void Kuplung_DX::Rendering::RenderingManager::StopTracking(const std::vector<std::unique_ptr<Kuplung_DX::Rendering::Models::Model3D>>& models) {
	switch (Kuplung_DX::App::RenderingMethod)
	{
	case 1:
		this->renderingSimple->StopTracking(models);
		break;
	default:
		break;
	}
}

bool Kuplung_DX::Rendering::RenderingManager::IsTracking(const std::vector<std::unique_ptr<Kuplung_DX::Rendering::Models::Model3D>>& models) {
	switch (Kuplung_DX::App::RenderingMethod)
	{
	case 1:
		return this->renderingSimple->IsTracking(models);
		return false;
		break;
	default:
		return false;
		break;
	}
}