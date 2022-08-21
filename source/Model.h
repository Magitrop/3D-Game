#pragma once
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <vector>
#include <string>
#include "Mesh.h";

class Mesh;
struct Texture;
class Model
{
private:
    friend class ModelRendererComponent;

    std::vector<Mesh*> meshes;
    std::string directory;

    void LoadModel(std::string path);
    void ProcessNode(aiNode* node, const aiScene* scene);
    Mesh* ProcessMesh(aiMesh* mesh, const aiScene* scene, const aiNode* node);
    std::vector<Texture> LoadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
public:
    Model(std::string path);
    Model(const std::vector<Mesh*>& _meshes = {});
};