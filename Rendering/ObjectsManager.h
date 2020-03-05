#pragma once

#include <Models/GUI/Grid.h>
#include <Models/Camera.h>

namespace Kuplung_DX
{
	namespace Rendering
	{
		class ObjectsManager
		{
		public:
			ObjectsManager(const std::shared_ptr<DX::DeviceResources>& deviceResources);
			~ObjectsManager();

			void Render();

			DirectX::XMFLOAT4X4 MatrixProjection;
			DirectX::XMFLOAT4X4 MatrixCamera;

			std::unique_ptr<Kuplung_DX::Models::Camera> CompCamera;
		private:
			std::shared_ptr<DX::DeviceResources> m_deviceResources;
			std::unique_ptr<Kuplung_DX::Models::GUI::Grid> compGrid;
		};
	}
}

