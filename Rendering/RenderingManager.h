#pragma once

#include "Models\ModelObject.h"
#include "Rendering\RenderingSimple.h"

namespace Kuplung_DX
{
	namespace Rendering
	{
		class RenderingManager
		{
		public:
			RenderingManager(const std::shared_ptr<DX::DeviceResources>& deviceResources);
			~RenderingManager();

			void Init();
			void Render(const std::vector<Kuplung_DX::Models::MeshModel>& models);

		private:
			std::unique_ptr<Kuplung_DX::Rendering::RenderingSimple> renderingSimple;

			// Cached pointer to device resources.
			std::shared_ptr<DX::DeviceResources> m_deviceResources;
		};
	}
}

