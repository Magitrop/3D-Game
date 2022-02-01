#pragma once
#include "../Initializer.h"

#define _USE_MATH_DEFINES
#include <cmath>
#include <algorithm>

#include <memory>
#include <vector>
#include <GLFW\glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "../GameObject/ObjectsManager.h"
#include "TransformComponent.h"
#include "../EventSystem.h"
#include "../Shaders/Shader.h"

class MeshRendererComponent : public Component
{
	COMPONENT(MeshRendererComponent)
protected:
	GLuint shaderProgramID;
	GLuint vertexArrayID;
	GLuint vertexBuffer;
	GLuint trianglesBuffer;
	GLuint normalBuffer;

	GLfloat* vertices = nullptr;
	size_t verticesCount = 0;

	GLuint* triangles = nullptr;
	size_t trianglesCount = 0;

	std::vector<Vector3> faceNormals;
	std::vector<Vector3> vertexNormals;

	void RecalculateFaceNormals();
	void RecalculateVertexNormals();
public:
	virtual void OnCreate() override;

	void SetVertices(const std::vector<Vector3>& newVertices);
	void SetTriangles(const std::vector<Vector3>& newTriangles);
	void Render();
};