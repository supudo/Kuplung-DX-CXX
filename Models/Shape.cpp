#include "pch.h"
#include "Shape.h"

Kuplung_DX::Models::Shape::Shape(Platform::String^ title, Platform::String^ filename) : title{ title }, filename{ filename } {
}

Platform::String^ Kuplung_DX::Models::Shape::getTitle() {
    return this->title;
}

Platform::String^ Kuplung_DX::Models::Shape::getFilename() {
    return this->filename;
}