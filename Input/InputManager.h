#pragma once

namespace Kuplung_DX
{
    namespace Input
    {
        ref class InputManager {
        internal:
            InputManager(_In_ Windows::UI::Core::CoreWindow^ window, _In_ Windows::UI::Core::CoreDispatcher^ dispatcher);
            InputManager(_In_ Windows::UI::Core::CoreWindow^ window);
            void InitWindow(_In_ Windows::UI::Core::CoreWindow^ window);

        private:
            Windows::UI::Core::CoreDispatcher^ m_dispatcher;
        };

    }
}