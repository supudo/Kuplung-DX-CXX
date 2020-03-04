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
		private:
			std::unique_ptr<Kuplung_DX::Models::Camera> compCamera;
			std::unique_ptr<Kuplung_DX::Models::GUI::Grid> compGrid;
		};
	}
}

