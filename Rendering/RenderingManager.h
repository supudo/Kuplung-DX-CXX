#pragma once

#include "Common\DeviceResources.h"
#include "Sample\ShaderStructures.h"
#include "Common\StepTimer.h"

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

			void Update(const std::vector<std::unique_ptr<Kuplung_DX::Rendering::Models::Model3D>>& models, DX::StepTimer const& timer);
			void Render(const std::vector<std::unique_ptr<Kuplung_DX::Rendering::Models::Model3D>>& models);
			void StartTracking(const std::vector<std::unique_ptr<Kuplung_DX::Rendering::Models::Model3D>>& models);
			void TrackingUpdate(const std::vector<std::unique_ptr<Kuplung_DX::Rendering::Models::Model3D>>& models, float positionX);
			void StopTracking(const std::vector<std::unique_ptr<Kuplung_DX::Rendering::Models::Model3D>>& models);
			bool IsTracking(const std::vector<std::unique_ptr<Kuplung_DX::Rendering::Models::Model3D>>& models);
			void Rotate(const std::vector<std::unique_ptr<Kuplung_DX::Rendering::Models::Model3D>>& models, float radians);

		private:
			std::unique_ptr<Kuplung_DX::Rendering::RenderingSimple> renderingSimple;
		};
	}
}

