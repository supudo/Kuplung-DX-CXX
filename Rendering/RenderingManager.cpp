#include "pch.h"
#include "RenderingManager.h"

Kuplung_DX::Rendering::RenderingManager::RenderingManager(const std::shared_ptr<DX::DeviceResources>& deviceResources) :
	m_deviceResources(deviceResources) {
	this->renderingSimple = std::make_unique<Kuplung_DX::Rendering::RenderingSimple>();
}

Kuplung_DX::Rendering::RenderingManager::~RenderingManager() {
	this->renderingSimple.reset();
}

void Kuplung_DX::Rendering::RenderingManager::Init() {
	this->renderingSimple->Init();
}

void Kuplung_DX::Rendering::RenderingManager::Render(const std::vector<Kuplung_DX::Models::MeshModel>& models) {
	this->renderingSimple->Render(models);
}