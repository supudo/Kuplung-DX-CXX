#pragma once

#include "Models\ModelObject.h"
#include "Importers\ObjParser.h"
#include <functional>

namespace Kuplung_DX
{
    namespace Importers
    {
        class FileModelManager {
        public:
            ~FileModelManager();
            FileModelManager();
            void init(const std::function<void(float)>& doProgress);
            std::vector<Kuplung_DX::Models::MeshModel> parse(std::string file, std::vector<std::string> settings);

        private:
            std::function<void(float)> funcProgress;
            void doProgress(float value);

            std::unique_ptr<Kuplung_DX::Importers::ObjParser> parserOBJ;
        };

    }
}