#include "ResourceManager.h"
#include "../Shaders/Shader.h"
#include "../Model.h"
#include <stdexcept>

std::map<std::string, Shader> ResourceManager::loadedShaders;
std::map<std::string, Model> ResourceManager::loadedModels;

void ResourceManager::UploadShader(const Shader& shaderToLoad, std::string shaderName)
{
    if (loadedShaders.find(shaderName) != loadedShaders.end())
        throw std::invalid_argument("Cannot load shader with name \"" + shaderName + "\" as it already exists.");
    loadedShaders[shaderName] = shaderToLoad;
}
const Shader& ResourceManager::GetShader(std::string shaderName)
{
    if (loadedShaders.find(shaderName) != loadedShaders.end())
        return loadedShaders[shaderName];
    else throw std::invalid_argument("There is no loaded shader with name \"" + shaderName + "\".");
}

void ResourceManager::UploadModel(const Model& modelToLoad, std::string modelName)
{
    if (loadedModels.find(modelName) != loadedModels.end())
        throw std::invalid_argument("Cannot load model with name \"" + modelName + "\" as it already exists.");
    loadedModels[modelName] = modelToLoad;
}
const Model& ResourceManager::GetModel(std::string modelName)
{
    if (loadedModels.find(modelName) != loadedModels.end())
        return loadedModels[modelName];
    else return Model();
}