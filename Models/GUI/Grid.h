#pragma once

#include <Types\Types.h>
#include <Rendering\ShaderStructures.h>

using namespace Kuplung_DX::Types;

namespace Kuplung_DX
{
    namespace Models
    {
        namespace GUI
        {
            class Grid
            {
            public:
                Grid(const std::shared_ptr<DX::DeviceResources>& deviceResources);
                ~Grid();
                void InitBuffers(const int& gridSize, const int& unitSize);
                void InitProperties();
                void Render(const DirectX::XMFLOAT4X4& matrixProjection, const DirectX::XMFLOAT4X4& matrixCamera);

                std::unique_ptr<ObjectCoordinate> PositionX, PositionY, PositionZ;
                std::unique_ptr<ObjectCoordinate> ScaleX, ScaleY, ScaleZ;
                std::unique_ptr<ObjectCoordinate> RotateX, RotateY, RotateZ;

                DirectX::XMMATRIX MatrixModel;

                std::vector<XMFLOAT3> dataVertices;
                std::vector<XMFLOAT3> dataColors;
                std::vector<UINT> dataIndices;

            private:
                std::shared_ptr<DX::DeviceResources> m_deviceResources;

                int gridSize, gridUnitSize;
                bool m_loadingComplete;
                uint32 m_indexCount;

                Microsoft::WRL::ComPtr<ID3D11VertexShader> m_vertexShader;
                Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pixelShader;

                Microsoft::WRL::ComPtr<ID3D11Buffer> m_vertexBuffer;
                Microsoft::WRL::ComPtr<ID3D11Buffer> m_indexBuffer;
                Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;

                Microsoft::WRL::ComPtr<ID3D11Buffer> m_constantBuffer;

                Kuplung_DX::Rendering::ModelViewProjectionConstantBuffer m_constantBufferData;

                void CreateDeviceDependentResources();
                void ReleaseDeviceDependentResources();
            };
        }
    }
}

