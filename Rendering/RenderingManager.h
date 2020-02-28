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

			void CreateDeviceDependentResources();
			void CreateWindowSizeDependentResources();
			void ReleaseDeviceDependentResources();
			void Update(DX::StepTimer const& timer);
			void Render(const std::vector<std::unique_ptr<Kuplung_DX::Rendering::Models::Model3D>>& models);
			void StartTracking();
			void TrackingUpdate(float positionX);
			void StopTracking();
			bool IsTracking();
			void Rotate(float radians);

		private:
			std::unique_ptr<Kuplung_DX::Rendering::RenderingSimple> renderingSimple;
		};
	}
}

