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
			RenderingManager();
			~RenderingManager();

			void Render(const std::vector<Kuplung_DX::Models::MeshModel>& models);

		private:
			std::unique_ptr<Kuplung_DX::Rendering::RenderingSimple> renderingSimple;
		};
	}
}

