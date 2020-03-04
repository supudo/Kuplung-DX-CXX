#include "pch.h"
#include "Camera.h"
#include "..\Common\DirectXHelper.h"

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
    this->PositionZ = std::make_unique<ObjectCoordinate>(false, -16.0f);

    this->RotateX = std::make_unique<ObjectCoordinate>(false, 160.0f);
    this->RotateY = std::make_unique<ObjectCoordinate>(false, 140.0f);
    this->RotateZ = std::make_unique<ObjectCoordinate>(false, 0.0f);

    this->RotateCenterX = std::make_unique<ObjectCoordinate>(false, 0.0f);
    this->RotateCenterY = std::make_unique<ObjectCoordinate>(false, 0.0f);
    this->RotateCenterZ = std::make_unique<ObjectCoordinate>(false, 0.0f);

    XMStoreFloat4x4(&this->MatrixCamera, DirectX::XMMatrixIdentity());
}

void Camera::Render() {
    XMStoreFloat4x4(&this->MatrixCamera, XMMatrixTranspose(XMMatrixLookAtRH(this->EyeSettings->View_Eye, this->EyeSettings->View_Center, this->EyeSettings->View_Up)));

    //this->matrixCamera = glm::translate(this->matrixCamera, glm::vec3(this->positionX->point, this->positionY->point, this->positionZ->point));

    //this->matrixCamera = glm::translate(this->matrixCamera, glm::vec3(0, 0, 0));
    //this->matrixCamera = glm::rotate(this->matrixCamera, glm::radians(this->rotateX->point), glm::vec3(1, 0, 0));
    //this->matrixCamera = glm::rotate(this->matrixCamera, glm::radians(this->rotateY->point), glm::vec3(0, 1, 0));
    //this->matrixCamera = glm::rotate(this->matrixCamera, glm::radians(this->rotateZ->point), glm::vec3(0, 0, 1));
    //this->matrixCamera = glm::translate(this->matrixCamera, glm::vec3(0, 0, 0));

    //this->matrixCamera = glm::rotate(this->matrixCamera, glm::radians(this->rotateCenterX->point), glm::vec3(1, 0, 0));
    //this->matrixCamera = glm::rotate(this->matrixCamera, glm::radians(this->rotateCenterY->point), glm::vec3(0, 1, 0));
    //this->matrixCamera = glm::rotate(this->matrixCamera, glm::radians(this->rotateCenterZ->point), glm::vec3(0, 0, 1));

    this->CameraPosition = DirectX::XMFLOAT3(this->MatrixCamera(3, 0), this->MatrixCamera(3, 1), this->MatrixCamera(3, 2));
}