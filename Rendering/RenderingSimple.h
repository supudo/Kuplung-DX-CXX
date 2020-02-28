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

		private:
		};
	}
}

