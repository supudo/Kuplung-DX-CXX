#include "pch.h"
#include "GuiObject.h"

Kuplung_DX::Models::GuiObject::GuiObject(uint32 id, Platform::String^ title, Platform::String^ icon) : id{ id }, title { title }, icon{ icon } {
}

uint32 Kuplung_DX::Models::GuiObject::getId() {
    return this->id;
}

Platform::String^ Kuplung_DX::Models::GuiObject::getTitle() {
    return this->title;
}

Platform::String^ Kuplung_DX::Models::GuiObject::getIcon() {
    return this->icon;
}