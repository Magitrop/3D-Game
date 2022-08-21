#define _USE_MATH_DEFINES
#include <cmath>

#ifdef __gl_h_
#undef __gl_h_
#endif // __gl_h_
#include <glad/glad.h>

#include <filesystem>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <chrono>

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "PerlinNoise.hpp"
#include "Controllers/Initializer.h"
#include "Controllers/EventSystem.h"
#include "Controllers/LightingController.h"
#include "GameObject/ObjectsManager.h"
#include "Math/Vectors.h"
#include "Shaders/Shader.h"
#include "Components/ModelRendererComponent.h"
#include "Components/TextRendererComponent.h"
#include "Components/TransformComponent.h"
#include "Components/CameraComponent.h"
#include "Controllers/ResourceManager.h"

using glm::Matrix4x4;

void RenderText(const Shader& shader, std::string text, float x, float y, float scale, Color color);

unsigned int VAO, VBO;

void LoadShaders()
{
	ResourceManager::UploadShader(Shader("..\\source\\TextVertexShader.vertexshader", "..\\source\\TextFragmentShader.fragmentshader"), "Text");
	ResourceManager::UploadShader(Shader("..\\source\\SimpleVertexShader.vertexshader", "..\\source\\SimpleFragmentShader.fragmentshader"), "Simple");
	//Shader::UploadShader(Shader("..\\source\\TextureVertShader.vertexshader", "..\\source\\TextureFragmentShader.fragmentshader"), "Map");
	ResourceManager::UploadShader(Shader("..\\source\\DepthShader.vertexshader", "..\\source\\DepthFragShader.fragmentshader"), "Depth");
	ResourceManager::UploadShader(Shader("..\\source\\ShadowsVert.vertexshader", "..\\source\\ShadowsFrag.fragmentshader"), "Shadows");
}

void LoadModels()
{
	ResourceManager::UploadModel(Model("C:/Users/Schweppes_psina/Desktop/castle.fbx"), "Castle");
}

int main()
{
	if (!Initializer.Init())
		return -1;

	auto camera = ObjectsManager::Instantiate<CameraComponent>(Vector3(0, 0, -3));
	EventSystem::SetAsMainCamera(camera);
	LoadShaders();
	LoadModels();

	LightingController::depthShader = &ResourceManager::GetShader("Depth");
	LightingController::lightPos = Vector3(-4, 4, 5);
	LightingController::Initialize();

	std::vector<Texture> textures = { };
	ModelRendererComponent* meshObj1 = ObjectsManager::Instantiate<ModelRendererComponent>();
	meshObj1->SetModel(&ResourceManager::GetModel("Castle"));
	meshObj1->SetShader(&ResourceManager::GetShader("Shadows"));
	meshObj1->gameObject->transform->Scale({ 0.001f, 0.001f, 0.001f });
	meshObj1->gameObject->transform->Translate({ 1, 0, 4 });

	std::vector<Vertex> vertices;
	std::vector<unsigned int> triangles;

	const siv::PerlinNoise::seed_type seed = 123456u;
	const siv::PerlinNoise perlin { seed };

	int sizeX = 30,
		sizeZ = 30;
	float scale = 0.05f;

	vertices.reserve((sizeX + 1) * (sizeZ + 1));
	std::map<std::pair<int, int>, std::vector<Vertex*>> tilePositions;
	for (int z = 0; z <= sizeZ; z++)
		for (int x = 0; x <= sizeX; x++)
		{
			vertices.push_back(Vector3(x * scale, 0, z * scale));
			Vertex* v = &vertices[vertices.size() - 1];
			tilePositions[std::make_pair(x / 2, z / 2)].push_back(v);
		}
	float height = 1.f;//perlin.octave2D_01(x * 0.1, z * 0.1f, 4) * 3;
	for (auto v : tilePositions[std::make_pair(2, 2)])
		v->Position.y = height;
	/*for (auto p : tilePositions)
	{
		auto& [x, z] = p.first;
		float height = -(x * z) * 0.01f;//perlin.octave2D_01(x * 0.1, z * 0.1f, 4) * 3;
		for (auto v : p.second)
			v->Position.y = height;
	}*/
	triangles.resize(sizeX * sizeZ * 6);
	for (int ti = 0, vi = 0, z = 0; z < sizeZ; z++, vi++) 
	{
		for (int x = 0; x < sizeX; x++, ti += 6, vi++) 
		{
			triangles[ti] = vi;
			triangles[ti + 3] = triangles[ti + 2] = vi + 1;
			triangles[ti + 4] = triangles[ti + 1] = vi + sizeX + 1;
			triangles[ti + 5] = vi + sizeX + 2;
		}
	}
	ModelRendererComponent* meshObj2 = ObjectsManager::Instantiate<ModelRendererComponent>();
	meshObj2->SetModel(new Model({ new Mesh(vertices, triangles, textures) }));
	meshObj2->SetShader(&ResourceManager::GetShader("Shadows"));
	meshObj2->gameObject->transform->Scale(Vector3(10, 1, 10));

	Matrix4x4 projection = glm::ortho(0.0f, Initializer.windowSize.x, 0.0f, Initializer.windowSize.y);

	std::vector<ModelRendererComponent*> meshesWithShadows { meshObj1, meshObj2 };

	float t = 0;
	while (!glfwWindowShouldClose(Initializer.window))
	{
		LightingController::PrepareDepthMap(meshesWithShadows);

		glViewport(0, 0, Initializer.windowSize.x, Initializer.windowSize.y);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		RenderText(
			ResourceManager::GetShader("Text"),
			Vectors::VectorToString(camera->gameObject->transform->GetRotation()), 
			0.0f, 10.0f, 0.75f, Color(1.0f, 1.0f, 1.0f, 1.0f));
		RenderText(
			ResourceManager::GetShader("Text"),
			Vectors::VectorToString(camera->gameObject->transform->GetPosition()),
			0.0f, 40.0f, 0.75f, Color(1.0f, 1.0f, 1.0f, 1.0f));
		glUniformMatrix4fv(glGetUniformLocation(ResourceManager::GetShader("Text").ID, "projection"), 1, GL_FALSE, &projection[0][0]);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, LightingController::GetDepthMapID());
		ResourceManager::GetShader("Shadows").SetInt("shadowMap", 0);
		for (auto obj : ObjectsManager::renderQueue)
			obj->Render();

		EventSystem::Update();

		for (long long i = 0; i < 1999999; ++i);

		glfwSwapBuffers(Initializer.window);
		glfwPollEvents();
	}

	Initializer.Quit();
	return 0;
}

void RenderText(const Shader& shader, std::string text, float x, float y, float scale, Color color)
{
	// activate corresponding render state	
	shader.Use();
	glUniform3f(glGetUniformLocation(shader.ID, "textColor"), color.x, color.y, color.z);
	glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(VAO);

	// iterate through all characters
	std::string::const_iterator c;
	for (c = text.begin(); c != text.end(); c++)
	{
		TextHandler::Character ch = TextHandler::characters[*c];

		float xpos = x + ch.Bearing.x * scale;
		float ypos = y - (ch.Size.y - ch.Bearing.y) * scale;

		float w = ch.Size.x * scale;
		float h = ch.Size.y * scale;
		// update VBO for each character
		float vertices[6][4] = 
		{
			{ xpos,     ypos + h,   0.0f, 0.0f },
			{ xpos,     ypos,       0.0f, 1.0f },
			{ xpos + w, ypos,       1.0f, 1.0f },

			{ xpos,     ypos + h,   0.0f, 0.0f },
			{ xpos + w, ypos,       1.0f, 1.0f },
			{ xpos + w, ypos + h,   1.0f, 0.0f }
		};
		// render glyph texture over quad
		glBindTexture(GL_TEXTURE_2D, ch.TextureID);
		// update content of VBO memory
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); // be sure to use glBufferSubData and not glBufferData

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		// render quad
		glDrawArrays(GL_TRIANGLES, 0, 6);
		// now advance cursors for next glyph (note that advance is number of 1/64 pixels)
		x += (ch.Advance >> 6) * scale; // bitshift by 6 to get value in pixels (2^6 = 64 (divide amount of 1/64th pixels by 64 to get amount of pixels))
	}
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}