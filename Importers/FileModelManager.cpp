#include "pch.h"
#include "Importers\FileModelManager.h"

Kuplung_DX::Importers::FileModelManager::~FileModelManager() {
    this->parserOBJ.reset();
}

Kuplung_DX::Importers::FileModelManager::FileModelManager() {
    this->parserOBJ = std::make_unique<ObjParser>();
}

void Kuplung_DX::Importers::FileModelManager::init(const std::function<void(float)>& doProgress) {
    this->funcProgress = doProgress;
    this->parserOBJ->init(std::bind(&FileModelManager::doProgress, this, std::placeholders::_1));
}

std::vector<Kuplung_DX::Models::MeshModel> Kuplung_DX::Importers::FileModelManager::parse(std::string file, std::vector<std::string> settings) {
    std::vector<Kuplung_DX::Models::MeshModel> meshModels;
    meshModels = this->parserOBJ->parse(file, settings);
    return meshModels;
}

void Kuplung_DX::Importers::FileModelManager::doProgress(float value) {
    this->funcProgress(value);
}
