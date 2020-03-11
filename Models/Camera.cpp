#include "pch.h"
#include "Camera.h"
#include <DX\DirectXHelper.h>
#include <Utilities/MathUtils.h>

using namespace Kuplung_DX::Models;

Camera::Camera() {}

Camera::~Camera() {
    this->EyeSettings.reset();

    this->PositionX.reset();
    this->PositionY.reset();
    this->PositionZ.reset();

    this->RotateX.reset();
    this->RotateY.reset();
    this->RotateZ.reset();

    this->RotateCenterX.reset();
    this->RotateCenterY.reset();
    this->RotateCenterZ.reset();
}

void Camera::InitProperties() {
    this->EyeSettings = std::make_unique<ObjectEye>();
    
    this->EyeSettings->View_Eye = { 0.0f, 0.7f, 1.5f, 0.0f };
    this->EyeSettings->View_Center = { 0.0f, -0.1f, 0.0f, 0.0f };
    this->EyeSettings->View_Up = { 0.0f, 1.0f, 0.0f, 0.0f };

    this->PositionX = std::make_unique<ObjectCoordinate>(false, 0.0f);
    this->PositionY = std::make_unique<ObjectCoordinate>(false, 0.0f);
    this->PositionZ = std::make_unique<ObjectCoordinate>(false, 0.0f);

    this->RotateX = std::make_unique<ObjectCoordinate>(false, 0.0f);
    this->RotateY = std::make_unique<ObjectCoordinate>(false, 0.0f);
    this->RotateZ = std::make_unique<ObjectCoordinate>(false, 0.0f);

    this->RotateCenterX = std::make_unique<ObjectCoordinate>(false, 0.0f);
    this->RotateCenterY = std::make_unique<ObjectCoordinate>(false, 0.0f);
    this->RotateCenterZ = std::make_unique<ObjectCoordinate>(false, 0.0f);

    XMStoreFloat4x4(&this->MatrixCamera, DirectX::XMMatrixIdentity());
}

void Camera::Render() {
    XMMATRIX mtxLookAt = XMMatrixTranspose(XMMatrixLookAtRH(this->EyeSettings->View_Eye, this->EyeSettings->View_Center, this->EyeSettings->View_Up));

    XMMATRIX mtxTransform = XMMatrixTranslation(this->PositionX->point, this->PositionY->point, this->PositionZ->point);
    
    XMMATRIX rcX = XMMatrixRotationX(XMConvertToRadians(this->RotateCenterX->point));
    XMMATRIX rcY = XMMatrixRotationY(XMConvertToRadians(this->RotateCenterY->point));
    XMMATRIX rcZ = XMMatrixRotationZ(XMConvertToRadians(this->RotateCenterZ->point));
    XMMATRIX mtxRotateCenter = rcX * rcY * rcZ;

    XMMATRIX mC = XMMatrixTranslation(0, 0, 0);
    XMMATRIX rX = XMMatrixRotationX(XMConvertToRadians(this->RotateX->point));
    XMMATRIX rY = XMMatrixRotationY(XMConvertToRadians(this->RotateY->point));
    XMMATRIX rZ = XMMatrixRotationZ(XMConvertToRadians(this->RotateZ->point));
    XMMATRIX mtxRotate = mC * rX * rY * rZ;

    XMStoreFloat4x4(&this->MatrixCamera, mtxLookAt * mtxTransform * mtxRotateCenter * mtxRotate);

    //Kuplung_DX::Utilities::MathUtils::PrettyPrintMatrix4x4(this->MatrixCamera);
    this->CameraPosition = DirectX::XMFLOAT3(this->MatrixCamera(3, 0), this->MatrixCamera(3, 1), this->MatrixCamera(3, 2));
}
