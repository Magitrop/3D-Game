#pragma once
#include "Components/ModelRendererComponent.h"
#include <vector>
#include <string>

struct Vertex final 
{
    Vector3 Position = Vector3();
    Vector3 Normal = Vector3();
    Vector2 TexCoords = Vector2();
    
    Vertex(
        Vector3 pos = Vector3(), 
        Vector3 normal = Vector3(), 
        Vector3 texcords = Vector3()) 
        : Position(pos), Normal(normal), TexCoords(texcords) {}
};

struct Texture final 
{
    unsigned int id;
    std::string type;
};

class Shader;
class Mesh final 
{
public:
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;

    Mesh(std::vector<Vertex>& vertices, std::vector<unsigned int>& indices, std::vector<Texture>& textures);
    Mesh(std::vector<Vertex>& vertices, std::vector<Vector3>& triangles, std::vector<Texture>& textures);
private:
    friend class ModelRendererComponent;
    unsigned int VAO, VBO, EBO;

    void SetupMesh();
    void RecalculateVertexNormals();
    std::vector<GLfloat> GetVertexPositions();
    std::vector<Vector3> GetVertexNormals();
    void Render(const Shader& shader);
};
