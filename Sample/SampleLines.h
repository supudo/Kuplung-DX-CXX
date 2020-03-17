#pragma once

#include <DX/DeviceResources.h>
#include "ShaderStructures.h"
#include <DX/StepTimer.h>

namespace Kuplung_DX {
	namespace Sample {
		class SampleLines {
		public:
			SampleLines(const std::shared_ptr<DX::DeviceResources>& deviceResources);
			void CreateDeviceDependentResources();
			void CreateWindowSizeDependentResources();
			void ReleaseDeviceDependentResources();
			void Update(DX::StepTimer const& timer);
			void Render(bool useKuplungMatrix, const DirectX::XMFLOAT4X4 matrixProjection, const DirectX::XMFLOAT4X4 matrixCamera);
			void StartTracking();
			void TrackingUpdate(float positionX);
			void StopTracking();
			bool IsTracking() { return m_tracking; }

		private:
			void Rotate(float radians);

		private:
			std::shared_ptr<DX::DeviceResources> m_deviceResources;

			// Direct3D resources for cube geometry.
			Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;
			Microsoft::WRL::ComPtr<ID3D11Buffer> m_vertexBuffer;
			Microsoft::WRL::ComPtr<ID3D11Buffer> m_indexBuffer;
			Microsoft::WRL::ComPtr<ID3D11VertexShader> m_vertexShader;
			Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pixelShader;
			Microsoft::WRL::ComPtr<ID3D11Buffer> m_constantBuffer;

			ModelViewProjectionConstantBuffer m_constantBufferData;
			uint32 m_indexCount;

			bool m_loadingComplete;
			float m_degreesPerSecond;
			bool m_tracking;
		};
	}
}

