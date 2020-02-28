#pragma once

#include <Models\ModelObject.h>
#include <Rendering\ShaderStructures.h>

namespace Kuplung_DX
{
    namespace Rendering
    {
        namespace Models
        {
            class Model3D
            {
            public:
                Model3D(const std::shared_ptr<DX::DeviceResources>& deviceResources);

                void InitModel3D(const Kuplung_DX::Models::MeshModel& model);

                void CreateDeviceDependentResources();
                void CreateWindowSizeDependentResources();
                void ReleaseDeviceDependentResources();
                void Render();

                void Update(DX::StepTimer const& timer);
                void StartTracking();
                void TrackingUpdate(float positionX);
                void StopTracking();
                bool IsTracking() { return m_tracking; }
                void Rotate(float radians);

                Kuplung_DX::Models::MeshModel MeshModel;

            private:
                std::shared_ptr<DX::DeviceResources> m_deviceResources;

                Microsoft::WRL::ComPtr<ID3D11VertexShader> m_vertexShader;
                Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pixelShader;

                Microsoft::WRL::ComPtr<ID3D11Buffer> m_vertexBuffer;
                Microsoft::WRL::ComPtr<ID3D11Buffer> m_indexBuffer;
                Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;

                Microsoft::WRL::ComPtr<ID3D11Buffer> m_constantBuffer;

                Kuplung_DX::Rendering::ModelViewProjectionConstantBuffer m_constantBufferData;
                uint32 m_indexCount;
                bool m_loadingComplete;

                float m_degreesPerSecond;
                bool m_tracking;
            };
        }
    }
}

