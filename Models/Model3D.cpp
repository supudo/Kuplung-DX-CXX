#include "pch.h"
#include "Model3D.h"

Kuplung_DX::Models::Model3D::Model3D(Platform::String^ title, Platform::String^ filename) : title{ title }, filename{ filename } {
}

Platform::String^ Kuplung_DX::Models::Model3D::getTitle() {
    return this->title;
}

Platform::String^ Kuplung_DX::Models::Model3D::getFilename() {
    return this->filename;
}