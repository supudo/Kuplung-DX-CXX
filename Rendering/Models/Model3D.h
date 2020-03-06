#pragma once

#include <Types\Types.h>
#include <Models\ModelObject.h>
#include <Rendering\ShaderStructures.h>

using namespace Kuplung_DX::Types;

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
                void InitProperties();

                void CreateDeviceDependentResources();
                void ReleaseDeviceDependentResources();
                void Render(const DirectX::XMFLOAT4X4 matrixProjection, const DirectX::XMFLOAT4X4 matrixCamera);

                void Update(DX::StepTimer const& timer);
                void StartTracking();
                void TrackingUpdate(float positionX);
                void StopTracking();
                bool IsTracking() { return m_tracking; }
                void Rotate(float radians);

                std::unique_ptr<ObjectCoordinate> PositionX, PositionY, PositionZ;
                std::unique_ptr<ObjectCoordinate> ScaleX, ScaleY, ScaleZ;
                std::unique_ptr<ObjectCoordinate> RotateX, RotateY, RotateZ;

                DirectX::XMMATRIX MatrixModel;
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

