#pragma once
#include <string>
#include <map>

class Shader;
class Model;
class ResourceManager final
{
private:
	ResourceManager() = delete;

	static std::map<std::string, Shader> loadedShaders;
	static std::map<std::string, Model> loadedModels;
public:
	static void UploadShader(const Shader& shaderToLoad, std::string shaderName);
	static const Shader& GetShader(std::string shaderName);

	static void UploadModel(const Model& shaderToLoad, std::string shaderName);
	static const Model& GetModel(std::string shaderName);
};