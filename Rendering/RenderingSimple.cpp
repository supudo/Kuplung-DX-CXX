#include "pch.h"
#include "RenderingSimple.h"

using namespace Kuplung_DX::Rendering;

RenderingSimple::RenderingSimple() {
}

void RenderingSimple::Render(const std::vector<std::unique_ptr<Kuplung_DX::Rendering::Models::Model3D>>& models) {
	for (auto & m : models) {
		m->Render();
	}
}