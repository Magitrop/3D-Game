#pragma once

#ifdef __gl_h_
#undef __gl_h_
#endif // __gl_h_
#include <glad/glad.h>

#include "MeshRendererComponent.h"

void MeshRendererComponent::RecalculateFaceNormals()
{
	faceNormals.clear();
	for (int i = 0; i < trianglesCount; i++)
	{
		Vector3 a
		(
			vertices[3 * triangles[3 * i]],
			vertices[3 * triangles[3 * i] + 1],
			vertices[3 * triangles[3 * i] + 2]
		);
		Vector3 b
		(
			vertices[3 * triangles[3 * i + 1]],
			vertices[3 * triangles[3 * i + 1] + 1],
			vertices[3 * triangles[3 * i + 1] + 2]
		);
		Vector3 c
		(
			vertices[3 * triangles[3 * i + 2]],
			vertices[3 * triangles[3 * i + 2] + 1],
			vertices[3 * triangles[3 * i + 2] + 2]
		);
		Vector3 n = glm::normalize(glm::cross(b - a, c - a));
		faceNormals.push_back(n);
	}
}
void MeshRendererComponent::RecalculateVertexNormals()
{
	vertexNormals.clear();
	for (int i = 0; i < verticesCount; i++)
	{
		std::vector<int> trianglesUsingThisVertex;
		for (int j = 0; j < 3 * trianglesCount; j += 3)
			for (int k = 0; k < 3; k++)
				// if triangle contains this vertex
				if (i == triangles[j + k])
				{
					trianglesUsingThisVertex.push_back(j / 3);
					break;
				}
		Vector3 n = Vector3();
		for (int j = 0; j < trianglesUsingThisVertex.size(); j++)
			n += faceNormals[trianglesUsingThisVertex[j]];
		vertexNormals.push_back(glm::normalize(n));
	}
}
void MeshRendererComponent::OnCreate()
{
	// delete previous vertex array
	if (glIsVertexArray(vertexArrayID))
		glDeleteVertexArrays(1, &vertexArrayID);
	// generate new vertex array
	glGenVertexArrays(1, &vertexArrayID);
}

void MeshRendererComponent::SetVertices(const std::vector<Vector3>& newVertices)
{
	auto count = verticesCount = newVertices.size();
	if (vertices != nullptr)
		delete vertices;
	vertices = new GLfloat[count * 3];

	for (size_t i = 0; i < count; i++)
	{
		vertices[i * 3] = newVertices[i].x;
		vertices[i * 3 + 1] = newVertices[i].y;
		vertices[i * 3 + 2] = newVertices[i].z;
	}

	glBindVertexArray(vertexArrayID);
	//
	if (glIsBuffer(vertexBuffer))
		glDeleteBuffers(1, &vertexBuffer);
	glGenBuffers(1, &vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, verticesCount * 3 * sizeof(GLfloat), vertices, GL_STATIC_DRAW);
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
	//
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}
void MeshRendererComponent::SetTriangles(const std::vector<Vector3>& newTriangles)
{
	auto count = trianglesCount = newTriangles.size();
	if (triangles != nullptr)
		delete vertices;
	triangles = new GLuint[count * 3];

	for (size_t i = 0; i < count; i++)
	{
		if (newTriangles[i].x > verticesCount ||
			newTriangles[i].y > verticesCount ||
			newTriangles[i].z > verticesCount)
			throw std::out_of_range("Vertex index was out of range.");

		triangles[i * 3] = newTriangles[i].x;
		triangles[i * 3 + 1] = newTriangles[i].y;
		triangles[i * 3 + 2] = newTriangles[i].z;
	}

	glBindVertexArray(vertexArrayID);
	//
	if (glIsBuffer(trianglesBuffer))
		glDeleteBuffers(1, &trianglesBuffer);
	glGenBuffers(1, &trianglesBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, trianglesBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(trianglesBuffer), triangles, GL_STATIC_DRAW);

	RecalculateFaceNormals();
	RecalculateVertexNormals();

	if (glIsBuffer(normalBuffer))
		glDeleteBuffers(1, &normalBuffer);
	glGenBuffers(1, &normalBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertexNormals.size() * sizeof(Vector3), &vertexNormals[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer
	(
		2,
		3,
		GL_FLOAT,
		GL_FALSE,
		0,
		(void*)0
	);
	//
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void MeshRendererComponent::SetShader(Shader* newShader)
{
	currentShader = newShader;
}

void MeshRendererComponent::Render()
{
	currentShader->Use();

	// attach MVP matrix
	glUniformMatrix4fv(
		glGetUniformLocation(currentShader->ID, "MVP"),
		1,
		GL_FALSE,
		&gameObject->transform->GetMVPMatrix()[0][0]);

	// attach light direction vector
	glUniform3fv(
		glGetUniformLocation(currentShader->ID, "lightDirection"),
		1,
		&light[0]);

	glBindVertexArray(vertexArrayID);
	glDrawElements(GL_TRIANGLES, trianglesCount * 3, GL_UNSIGNED_INT, triangles);
	glBindVertexArray(0);

	glUseProgram(0);
}

void MeshRendererComponent::RenderDepth(Shader* depthShader)
{
	depthShader->Use();

	glBindVertexArray(vertexArrayID);
	glDrawElements(GL_TRIANGLES, trianglesCount * 3, GL_UNSIGNED_INT, triangles);
	glBindVertexArray(0);

	glUseProgram(0);
}
