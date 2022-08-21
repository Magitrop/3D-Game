#include <glad/glad.h>
#include "Shaders/Shader.h"
#include "Mesh.h"

Mesh::Mesh(std::vector<Vertex>& vertices, std::vector<unsigned int>& indices, std::vector<Texture>& textures)
{
    this->vertices = vertices;
    this->indices = indices;
    this->textures = textures;

    SetupMesh();
}

void Mesh::RecalculateVertexNormals()
{
    std::vector<Vector3> faceNormals;
    for (int i = 0; i < indices.size(); i += 3)
    {
        Vector3 a
        (
            vertices[indices[i]].Position.x,
            vertices[indices[i]].Position.y,
            vertices[indices[i]].Position.z
        );
        Vector3 b
        (
            vertices[indices[i + 1]].Position.x,
            vertices[indices[i + 1]].Position.y,
            vertices[indices[i + 1]].Position.z
        );
        Vector3 c
        (
            vertices[indices[i + 2]].Position.x,
            vertices[indices[i + 2]].Position.y,
            vertices[indices[i + 2]].Position.z
        );
        Vector3 n = glm::normalize(glm::cross(b - a, c - a));
        faceNormals.push_back(n);
    }

    for (int i = 0; i < vertices.size(); i++)
    {
        std::vector<int> trianglesUsingThisVertex;
        for (int j = 0; j < indices.size(); j += 3)
            for (int k = 0; k < 3; k++)
                // if triangle contains this vertex
                if (i == indices[j + k])
                {
                    trianglesUsingThisVertex.push_back(j / 3);
                    break;
                }
        Vector3 n = Vector3();
        for (int j = 0; j < trianglesUsingThisVertex.size(); j++)
        {
            n += faceNormals[trianglesUsingThisVertex[j]];
        }
        vertices[i].Normal = glm::normalize(n);
    }
}

void Mesh::Render(const Shader& shader)
{
    unsigned int diffuseNr = 1;
    unsigned int specularNr = 1;
    for (unsigned int i = 0; i < textures.size(); i++)
    {
        glActiveTexture(GL_TEXTURE0 + i); // activate proper texture unit before binding
        // retrieve texture number (the N in diffuse_textureN)
        std::string number;
        std::string name = textures[i].type;
        if (name == "texture_diffuse")
            number = std::to_string(diffuseNr++);
        else if (name == "texture_specular")
            number = std::to_string(specularNr++);
    
        shader.SetFloat(("material." + name + number).c_str(), i);
        glBindTexture(GL_TEXTURE_2D, textures[i].id);
    }
    glActiveTexture(GL_TEXTURE0);

    // draw mesh
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, &indices[0]);
    glBindVertexArray(0);
}

std::vector<GLfloat> Mesh::GetVertexPositions()
{
    std::vector<GLfloat> vertexPositions;
    for (auto& n : vertices)
    {
        vertexPositions.push_back(n.Position.x);
        vertexPositions.push_back(n.Position.y);
        vertexPositions.push_back(n.Position.z);
    }
    return vertexPositions;
}
std::vector<Vector3> Mesh::GetVertexNormals()
{
    std::vector<Vector3> vertexNormals;
    for (auto& n : vertices)
        vertexNormals.push_back(n.Normal);
    return vertexNormals;
}

void Mesh::SetupMesh()
{
    RecalculateVertexNormals();

    unsigned int vertexBuffer = 0;
    unsigned int trianglesBuffer = 0;
    unsigned int normalBuffer = 0;
    std::vector<Vector3> vertexNormals = GetVertexNormals();
    std::vector<GLfloat> vertexPositions = GetVertexPositions();

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    // 1
    if (glIsBuffer(vertexBuffer))
        glDeleteBuffers(1, &vertexBuffer);
    glGenBuffers(1, &vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vector3), &vertexPositions[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer
    (
        0,
        3,
        GL_FLOAT,
        GL_FALSE,
        0,
        (void*)0
    );
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // 2
    if (glIsBuffer(trianglesBuffer))
        glDeleteBuffers(1, &trianglesBuffer);
    glGenBuffers(1, &trianglesBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, trianglesBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

    // 3
    if (glIsBuffer(normalBuffer))
        glDeleteBuffers(1, &normalBuffer);
    glGenBuffers(1, &normalBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vector3), &vertexNormals[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer
    (
        1,
        3,
        GL_FLOAT,
        GL_FALSE,
        0,
        (void*)0
    );

    // 4
    std::vector<Vector2> texPositions
    {
        Vector2(0, 0),
        Vector2(0, 1),
        Vector2(1, 1),
    };
    GLuint texPosBuffer = 0;

    if (glIsBuffer(texPosBuffer))
        glDeleteBuffers(1, &texPosBuffer);
    glGenBuffers(1, &texPosBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, texPosBuffer);
    glBufferData(GL_ARRAY_BUFFER, texPositions.size() * sizeof(Vector2), &texPositions[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(2);
    glVertexAttribPointer
    (
        2,
        2,
        GL_FLOAT,
        GL_FALSE,
        0,
        (void*)0
    );
    //
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);

    //glBindBuffer(GL_ARRAY_BUFFER, VBO);

    //glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    //glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int),
    //    &indices[0], GL_STATIC_DRAW);

    //RecalculateVertexNormals();

    //// vertex positions
    //glEnableVertexAttribArray(0);
    //glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    //// vertex normals
    //glEnableVertexAttribArray(1);
    //glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
    //// vertex texture coords
    //glEnableVertexAttribArray(2);
    //glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));

    //glBindVertexArray(0);
}