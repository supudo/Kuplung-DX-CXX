#include "pch.h"
#include "InputManager.h"

using namespace Windows::UI::Core;

Kuplung_DX::Input::InputManager::InputManager(_In_ CoreWindow^ window, _In_ CoreDispatcher^ dispatcher) : m_dispatcher(dispatcher) {
    this->InitWindow(window);
}

Kuplung_DX::Input::InputManager::InputManager(_In_ CoreWindow^ window) : m_dispatcher(nullptr) {
    this->InitWindow(window);
}

void Kuplung_DX::Input::InputManager::InitWindow(_In_ CoreWindow^ window) {
}