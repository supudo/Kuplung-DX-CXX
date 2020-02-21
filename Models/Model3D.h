#pragma once

namespace Kuplung_DX
{
    namespace Models
    {
        public ref class Model3D sealed
        {
        public:
            Model3D(Platform::String^ Title, Platform::String^ filename);
            Platform::String^ getTitle();
            Platform::String^ getFilename();

            property Platform::String^ Title {
                Platform::String^ get() { return this->title; }
            }
            property Platform::String^ Filename {
                Platform::String^ get() { return this->filename; }
            }
        private:
            Platform::String^ title;
            Platform::String^ filename;
        };
    }
}

