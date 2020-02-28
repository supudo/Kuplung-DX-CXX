#pragma once

#include <Rendering\Models\Model3D.h>

namespace Kuplung_DX
{
	namespace Rendering
	{
		class RenderingSimple
		{
		public:
			RenderingSimple();
			void Render(const std::vector<std::unique_ptr<Kuplung_DX::Rendering::Models::Model3D>>& models);

			void Update(const std::vector<std::unique_ptr<Kuplung_DX::Rendering::Models::Model3D>>& models, DX::StepTimer const& timer);
			void StartTracking(const std::vector<std::unique_ptr<Kuplung_DX::Rendering::Models::Model3D>>& models);
			void TrackingUpdate(const std::vector<std::unique_ptr<Kuplung_DX::Rendering::Models::Model3D>>& models, float positionX);
			void StopTracking(const std::vector<std::unique_ptr<Kuplung_DX::Rendering::Models::Model3D>>& models);
			bool IsTracking(const std::vector<std::unique_ptr<Kuplung_DX::Rendering::Models::Model3D>>& models);
			void Rotate(const std::vector<std::unique_ptr<Kuplung_DX::Rendering::Models::Model3D>>& models, float radians);

		private:
		};
	}
}

