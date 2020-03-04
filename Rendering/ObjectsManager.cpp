#include "pch.h"
#include "ObjectsManager.h"

using namespace Kuplung_DX::Rendering;

ObjectsManager::ObjectsManager(const std::shared_ptr<DX::DeviceResources>& deviceResources) {
	this->compCamera = std::make_unique<Kuplung_DX::Models::Camera>();
	this->compCamera->InitProperties();

	this->compGrid = std::make_unique<Kuplung_DX::Models::GUI::Grid>(deviceResources);
	this->compGrid->InitBuffers(Kuplung_DX::App::GridSize, Kuplung_DX::App::GridUnitSize);
}

ObjectsManager::~ObjectsManager() {
	compGrid.reset();
}

void ObjectsManager::Render() {
	compCamera->Render();
	compGrid->Render(this->compCamera->MatrixProjection, this->compCamera->MatrixCamera);
}
