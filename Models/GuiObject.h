#pragma once

namespace Kuplung_DX
{
    namespace Models
    {
        public ref class GuiObject sealed
        {
        public:
            GuiObject(uint32 Id, Platform::String^ Title, Platform::String^ Icon);
            uint32 getId();
            Platform::String^ getTitle();
            Platform::String^ getIcon();
            
            property uint32 Id {
                uint32 get() { return this->id; }
            }
            property Platform::String^ Title {
                Platform::String^ get() { return this->title; }
            }
            property Platform::String^ Icon {
                Platform::String^ get() { return this->icon; }
            }
        private:
            uint32 id;
            Platform::String^ title;
            Platform::String^ icon;
        };
    }
}

