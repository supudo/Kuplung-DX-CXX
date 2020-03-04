#pragma once

namespace Kuplung_DX
{
    namespace Models
    {
        class Camera
        {
        public:
            Camera();
            ~Camera();
            void InitProperties();
            void Render();

            std::unique_ptr<ObjectEye> EyeSettings;
            std::unique_ptr<ObjectCoordinate> PositionX, PositionY, PositionZ;
            std::unique_ptr<ObjectCoordinate> RotateX, RotateY, RotateZ;
            std::unique_ptr<ObjectCoordinate> RotateCenterX, RotateCenterY, RotateCenterZ;

            DirectX::XMFLOAT4X4 MatrixCamera;
            DirectX::XMFLOAT3 CameraPosition;
        private:
        };
    }
}

