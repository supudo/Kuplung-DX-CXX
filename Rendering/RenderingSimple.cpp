#include "pch.h"
#include "RenderingSimple.h"

using namespace Kuplung_DX::Rendering;

RenderingSimple::RenderingSimple() {
}

void RenderingSimple::Render(const DirectX::XMFLOAT4X4 matrixProjection, const DirectX::XMFLOAT4X4 matrixCamera, const std::vector<std::unique_ptr<Kuplung_DX::Rendering::Models::Model3D>>& models) {
	for (auto & m : models) {
		m->Render(matrixProjection, matrixCamera);
	}
}

void RenderingSimple::Update(const std::vector<std::unique_ptr<Kuplung_DX::Rendering::Models::Model3D>>& models, DX::StepTimer const& timer) {
	for each (auto& m in models) {
		m->Update(timer);
	}
}

void RenderingSimple::Rotate(const std::vector<std::unique_ptr<Kuplung_DX::Rendering::Models::Model3D>>& models, float radians) {
	for each (auto & m in models) {
		m->Rotate(radians);
	}
}

void RenderingSimple::StartTracking(const std::vector<std::unique_ptr<Kuplung_DX::Rendering::Models::Model3D>>& models) {
	for each (auto & m in models) {
		m->StartTracking();
	}
}

void RenderingSimple::TrackingUpdate(const std::vector<std::unique_ptr<Kuplung_DX::Rendering::Models::Model3D>>& models, float positionX) {
	for each (auto & m in models) {
		m->TrackingUpdate(positionX);
	}
}

void RenderingSimple::StopTracking(const std::vector<std::unique_ptr<Kuplung_DX::Rendering::Models::Model3D>>& models) {
	for each (auto & m in models) {
		m->StopTracking();
	}
}

bool RenderingSimple::IsTracking(const std::vector<std::unique_ptr<Kuplung_DX::Rendering::Models::Model3D>>& models) {
	for each (auto & m in models) {
		if (m->IsTracking())
			return true;
	}
	return false;
}