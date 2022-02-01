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

#include "Controllers/Initializer.h"
#include "Controllers/EventSystem.h"
#include "Controllers/LightingController.h"
#include "GameObject/ObjectsManager.h"
#include "Math/Vectors.h"
#include "Shaders/Shader.h"
#include "Components/MeshRendererComponent.h"
#include "Components/TextRendererComponent.h"
#include "Components/TransformComponent.h"
#include "Components/CameraComponent.h"

using glm::Matrix4x4;

GLuint LoadShaders(const char* vertex_file_path, const char* fragment_file_path) {

	// Создаем шейдеры
	GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	// Загружаем код Вершинного Шейдера из файла
	std::string VertexShaderCode;
	std::ifstream VertexShaderStream(vertex_file_path, std::ios::in);
	if (VertexShaderStream.is_open())
	{
		std::stringstream sstr;
		sstr << VertexShaderStream.rdbuf();
		VertexShaderCode = sstr.str();
		VertexShaderStream.close();
	}

	// Загружаем код Фрагментного шейдера из файла
	std::string FragmentShaderCode;
	std::ifstream FragmentShaderStream(fragment_file_path, std::ios::in);
	if (FragmentShaderStream.is_open()) {
		std::stringstream sstr;
		sstr << FragmentShaderStream.rdbuf();
		FragmentShaderCode = sstr.str();
		FragmentShaderStream.close();
	}

	GLint Result = GL_FALSE;
	GLint InfoLogLength;

	// Компилируем Вершинный шейдер
	printf("compiling shader: %s\n", vertex_file_path);
	char const* VertexSourcePointer = VertexShaderCode.c_str();
	glShaderSource(VertexShaderID, 1, &VertexSourcePointer, NULL);
	glCompileShader(VertexShaderID);

	// Выполняем проверку Вершинного шейдера
	glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		std::vector<char> VertexShaderErrorMessage(InfoLogLength + 1);
		glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
		fprintf(stdout, "%s\n", &VertexShaderErrorMessage[0]);
	}

	// Компилируем Фрагментный шейдер
	printf("compiling shader: %s\n", fragment_file_path);
	char const* FragmentSourcePointer = FragmentShaderCode.c_str();
	glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer, NULL);
	glCompileShader(FragmentShaderID);

	// Проверяем Фрагментный шейдер
	glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		std::vector<char> FragmentShaderErrorMessage(InfoLogLength + 1);
		glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
		fprintf(stdout, "%s\n", &FragmentShaderErrorMessage[0]);
	}

	// Создаем шейдерную программу и привязываем шейдеры к ней
	fprintf(stdout, "attaching shaders\n");
	GLuint ProgramID = glCreateProgram();
	glAttachShader(ProgramID, VertexShaderID);
	glAttachShader(ProgramID, FragmentShaderID);
	glLinkProgram(ProgramID);

	// Проверяем шейдерную программу
	glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
	glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		std::vector<char> ProgramErrorMessage(InfoLogLength + 1);
		glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
		fprintf(stdout, "%s\n", &ProgramErrorMessage[0]);
	}

	glDeleteShader(VertexShaderID);
	glDeleteShader(FragmentShaderID);

	return ProgramID;
}

void RenderText(Shader& shader, std::string text, float x, float y, float scale, glm::vec3 color);

unsigned int VAO, VBO;

int main()
{
	if (!Initializer.Init())
		return -1;

	auto camera = ObjectsManager::Instantiate<CameraComponent>(Vector3(0, 0, -3));
	EventSystem::SetAsMainCamera(camera);

	// compile and setup the shader
	Shader shader("..\\source\\TextVertexShader.vertexshader", "..\\source\\TextFragmentShader.fragmentshader");
	Shader shader1("..\\source\\SimpleVertexShader.vertexshader", "..\\source\\SimpleFragmentShader.fragmentshader");
	Shader depthShader("..\\source\\DepthShader.vertexshader", "..\\source\\DepthFragShader.fragmentshader");

	LightingController::depthShader = &depthShader;
	LightingController::Initialize();

	// configure VAO/VBO for texture quads
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	MeshRendererComponent* mesh1 = ObjectsManager::Instantiate<MeshRendererComponent>();
	mesh1->SetVertices(
		{
			Vector3(0.0f, 0.0f, 0.0f),
			Vector3(0.0f, 1.0f, 0.0f),
			Vector3(1.0f, 1.0f, 0.0f),
			Vector3(1.0f, 0.0f, 0.0f),

			Vector3(0.0f, 0.0f, 1.0f),
			Vector3(0.0f, 1.0f, 1.0f),
			Vector3(1.0f, 1.0f, 1.0f),
			Vector3(1.0f, 0.0f, 1.0f)
		});
	mesh1->SetTriangles(
		{
			Vector3(0, 1, 2),
			Vector3(2, 3, 0),

			Vector3(3, 2, 6),
			Vector3(6, 7, 3),

			Vector3(7, 6, 5),
			Vector3(5, 4, 7),

			Vector3(4, 5, 1),
			Vector3(1, 0, 4),

			Vector3(1, 5, 6),
			Vector3(6, 2, 1),

			Vector3(4, 0, 3),
			Vector3(3, 7, 4)
		});
	mesh1->SetShader(&shader1);

	MeshRendererComponent* mesh2 = ObjectsManager::Instantiate<MeshRendererComponent>();
	mesh2->SetVertices(
		{
			Vector3(0.0f, 0.0f, 0.0f),
			Vector3(0.0f, 0.0f, 1.0f),
			Vector3(1.0f, 0.0f, 1.0f),
			Vector3(1.0f, 0.0f, 0.0f)
		});
	mesh2->SetTriangles(
		{
			Vector3(0, 1, 2),
			Vector3(2, 3, 0),
		});
	mesh2->SetShader(&shader1);
	mesh2->gameObject->transform->Scale(Vector3(10, 1, 10));
	mesh2->gameObject->transform->Translate(Vectors::up, -1.f);
	mesh2->gameObject->transform->Translate(Vectors::forward, -0.25f);
	mesh2->gameObject->transform->Translate(Vectors::right, -0.25f);

	glm::mat4 projection = glm::ortho(0.0f, Initializer.windowSize.x, 0.0f, Initializer.windowSize.y);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	std::vector<MeshRendererComponent*> meshesWithShadows { mesh1 };

	while (!glfwWindowShouldClose(Initializer.window))
	{
		LightingController::PrepareDepthMap(meshesWithShadows);

		glViewport(0, 0, Initializer.windowSize.x, Initializer.windowSize.y);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glBindTexture(GL_TEXTURE_2D, LightingController::GetDepthMapID());

		RenderText(
			shader, 
			Vectors::VectorToString(camera->gameObject->transform->GetRotation()), 
			0.0f, 10.0f, 0.75f, Vector3(1.0f, 1.0f, 1.0f));
		glUniformMatrix4fv(glGetUniformLocation(shader.ID, "projection"), 1, GL_FALSE, &projection[0][0]);

		mesh1->Render();
		mesh2->Render();

		EventSystem::Update();

		for (long long i = 0; i < 1999999; ++i);

		glfwSwapBuffers(Initializer.window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}

void RenderText(Shader& shader, std::string text, float x, float y, float scale, glm::vec3 color)
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