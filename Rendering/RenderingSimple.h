#pragma once

#include "Models\ModelObject.h"

namespace Kuplung_DX
{
	namespace Rendering
	{
		class RenderingSimple
		{
		public:
			RenderingSimple();
			~RenderingSimple();

			void Init();
			void Render(const std::vector<Kuplung_DX::Models::MeshModel>& models);
		};
	}
}

