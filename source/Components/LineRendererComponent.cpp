#include <glad/glad.h>

#include "LineRendererComponent.h"
#include "../GameObject/ObjectsManager.h"
#include "../Shaders/Shader.h"
#include "../Controllers/ResourceManager.h"
#include "../Controllers/EventSystem.h"

void LineRendererComponent::OnCreate()
{
    indices = { 0, 1 };

    std::vector<Vector3> vertices;
    vertices.push_back(Vector3(0, 0, 0));
    vertices.push_back(Vector3(-1, -1, -1));

    glGenVertexArrays(1, &VAO);

    glBindVertexArray(VAO);

    // 1
    if (glIsBuffer(vertexBuffer))
        glDeleteBuffers(1, &vertexBuffer);
    glGenBuffers(1, &vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vector3), &vertices[0], GL_STATIC_DRAW);
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

    //
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);
}

void LineRendererComponent::SetPoints(std::vector<Vector3>& points)
{
    glBindVertexArray(VAO);

    if (indices.size() == points.size())
    {
        // so there is no need to reallocate the buffer
        glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
        memcpy(
            glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY),
            &points[0],
            points.size() * sizeof(Vector3));
        glUnmapBuffer(GL_ARRAY_BUFFER);
    }
    else
    {
        indices.clear();
        for (int i = 0; i < points.size(); i++)
            indices.push_back(i);

        // 1
        if (glIsBuffer(vertexBuffer))
            glDeleteBuffers(1, &vertexBuffer);
        glGenBuffers(1, &vertexBuffer);
        glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
        glBufferData(GL_ARRAY_BUFFER, points.size() * sizeof(Vector3), &points[0], GL_STATIC_DRAW);
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
    }

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void LineRendererComponent::Render()
{
	Shader sh = ResourceManager::GetShader("Simple");
	sh.Use();
    sh.setMat4("MVP", EventSystem::GetMainCamera()->GetProjectionViewMatrix());
    sh.setVec3("objPos", gameObject->transform->GetPosition());
    sh.setVec4("vertexColor", color);

    glBindVertexArray(VAO);
    glLineWidth(lineWidth);
    glDrawElements(connectLines ? GL_LINE_STRIP : GL_LINES, indices.size(), GL_UNSIGNED_INT, &indices[0]);

    glBindVertexArray(0);
	glUseProgram(0);
}