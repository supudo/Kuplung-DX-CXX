#include "pch.h"
#include "Camera.h"
#include <DX\DirectXHelper.h>

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

    this->EyeSettings->View_Eye = { 0.6f, 0.4f, 1.5f, 0.0f };
    this->EyeSettings->View_Center = { 0.0f, -0.1f, 0.0f, 0.0f };
    this->EyeSettings->View_Up = { 0.0f, 1.0f, 0.0f, 0.0f };

    //this->EyeSettings->View_Eye = { 0.0f, 0.0f, 10.0f, 0.0f };
    //this->EyeSettings->View_Center = { 0.0f, 0.0f, 0.0f, 0.0f };
    //this->EyeSettings->View_Up = { 0.0f, -1.0f, 0.0f, 0.0f };

    this->PositionX = std::make_unique<ObjectCoordinate>(false, 0.0f);
    this->PositionY = std::make_unique<ObjectCoordinate>(false, 0.0f);
    this->PositionZ = std::make_unique<ObjectCoordinate>(false, 0.0f);

    this->RotateX = std::make_unique<ObjectCoordinate>(false, 122.0f);
    this->RotateY = std::make_unique<ObjectCoordinate>(false, 140.0f);
    this->RotateZ = std::make_unique<ObjectCoordinate>(false, 74.0f);

    this->RotateCenterX = std::make_unique<ObjectCoordinate>(false, 0.0f);
    this->RotateCenterY = std::make_unique<ObjectCoordinate>(false, 0.0f);
    this->RotateCenterZ = std::make_unique<ObjectCoordinate>(false, 0.0f);

    XMStoreFloat4x4(&this->MatrixCamera, DirectX::XMMatrixIdentity());
}

void Camera::Render() {
    XMStoreFloat4x4(&this->MatrixCamera, XMMatrixTranspose(XMMatrixLookAtLH(this->EyeSettings->View_Eye, this->EyeSettings->View_Center, this->EyeSettings->View_Up)));

    XMStoreFloat4x4(&this->MatrixCamera, XMLoadFloat4x4(&this->MatrixCamera) * XMMatrixTranslation(this->PositionX->point, this->PositionY->point, this->PositionZ->point));

    XMStoreFloat4x4(&this->MatrixCamera, XMLoadFloat4x4(&this->MatrixCamera) * XMMatrixRotationY(XMConvertToRadians(this->RotateX->point)));
    XMStoreFloat4x4(&this->MatrixCamera, XMLoadFloat4x4(&this->MatrixCamera) * XMMatrixRotationY(XMConvertToRadians(this->RotateY->point)));
    XMStoreFloat4x4(&this->MatrixCamera, XMLoadFloat4x4(&this->MatrixCamera) * XMMatrixRotationY(XMConvertToRadians(this->RotateZ->point)));

    //XMStoreFloat4x4(&this->MatrixCamera, XMMatrixTranspose(XMMatrixRotationRollPitchYaw(XMConvertToRadians(this->RotateCenterX->point), XMConvertToRadians(this->RotateCenterY->point), XMConvertToRadians(this->RotateCenterZ->point))));

    this->CameraPosition = DirectX::XMFLOAT3(this->MatrixCamera(3, 0), this->MatrixCamera(3, 1), this->MatrixCamera(3, 2));
}