#include "pch.h"
#include "RenderingManager.h"

using namespace Kuplung_DX::Rendering;
using namespace DirectX;
using namespace Windows::Foundation;

RenderingManager::RenderingManager(const std::shared_ptr<Kuplung_DX::DX::DeviceResources>& deviceResources) {
	this->renderingSimple = std::make_unique<RenderingSimple>();
}

void RenderingManager::Render(const DirectX::XMFLOAT4X4 matrixProjection, const DirectX::XMFLOAT4X4 matrixCamera, const std::vector<std::unique_ptr<Models::Model3D>>& models) {
	switch (Kuplung_DX::App::RenderingMethod)
	{
	case 1:
		this->renderingSimple->Render(matrixProjection, matrixCamera, models);
		break;
	default:
		break;
	}
}

void RenderingManager::Update(const std::vector<std::unique_ptr<Models::Model3D>>& models, Kuplung_DX::DX::StepTimer const& timer) {
	switch (Kuplung_DX::App::RenderingMethod)
	{
	case 1:
		this->renderingSimple->Update(models, timer);
		break;
	default:
		break;
	}
}

void RenderingManager::Rotate(const std::vector<std::unique_ptr<Models::Model3D>>& models, float radians) {
	switch (Kuplung_DX::App::RenderingMethod)
	{
	case 1:
		this->renderingSimple->Rotate(models, radians);
		break;
	default:
		break;
	}
}

void RenderingManager::StartTracking(const std::vector<std::unique_ptr<Models::Model3D>>& models) {
	switch (Kuplung_DX::App::RenderingMethod)
	{
	case 1:
		this->renderingSimple->StartTracking(models);
		break;
	default:
		break;
	}
}

void RenderingManager::TrackingUpdate(const std::vector<std::unique_ptr<Models::Model3D>>& models, float positionX) {
	switch (Kuplung_DX::App::RenderingMethod)
	{
	case 1:
		this->renderingSimple->TrackingUpdate(models, positionX);
		break;
	default:
		break;
	}
}

void RenderingManager::StopTracking(const std::vector<std::unique_ptr<Models::Model3D>>& models) {
	switch (Kuplung_DX::App::RenderingMethod)
	{
	case 1:
		this->renderingSimple->StopTracking(models);
		break;
	default:
		break;
	}
}

bool RenderingManager::IsTracking(const std::vector<std::unique_ptr<Models::Model3D>>& models) {
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