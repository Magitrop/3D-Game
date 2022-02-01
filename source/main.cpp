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

#include "Initializer.h"
#include "EventSystem.h"
#include "GameObject/ObjectsManager.h"
#include "Math/Vectors.h"
#include "Shaders/Shader.h"
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
	auto camera = ObjectsManager::Instantiate<CameraComponent>(Vector3(0, 0, 3));

	if (!Initializer.Init())
		return -1;

	// compile and setup the shader
	// ----------------------------
	Shader shader("..\\source\\TextVertexShader.vertexshader", "..\\source\\TextFragmentShader.fragmentshader");
	glm::mat4 projection = glm::ortho(0.0f, Initializer.windowSize.x, 0.0f, Initializer.windowSize.y);
	//shader.Use();
	//glUniformMatrix4fv(glGetUniformLocation(shader.ID, "projection"), 1, GL_FALSE, &projection[0][0]);

	// configure VAO/VBO for texture quads
	// -----------------------------------
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	GLuint programID = LoadShaders(
		"..\\source\\SimpleVertexShader.vertexshader",
		"..\\source\\SimpleFragmentShader.fragmentshader");

	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	const GLfloat vertices[] =
	{
		0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		1.0f, 1.0f, 0.0f,
		1.0f, 0.0f, 0.0f,

		0.0f, 0.0f, 1.0f,
		0.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 0.0f, 1.0f,
	};
	const GLuint triangles[] =
	{
		0, 1, 2,
		2, 3, 0,

		3, 2, 6,
		6, 7, 3,

		7, 6, 5,
		5, 4, 7,

		4, 5, 1,
		1, 0, 4,

		1, 5, 6,
		6, 2, 1,

		4, 0, 3,
		3, 7, 4,
	};
	std::vector<Vector3> faceNormals;
	std::vector<Vector3> vertexNormals;
	for (int i = 0; i < 12; i++)
	{
		Vector3 a = Vector3(vertices[3 * triangles[3 * i]], vertices[3 * triangles[3 * i] + 1], vertices[3 * triangles[3 * i] + 2]);
		//cout << "a: " << a.x << " " << a.y << " " << a.z << endl;
		Vector3 b = Vector3(vertices[3 * triangles[3 * i + 1]], vertices[3 * triangles[3 * i + 1] + 1], vertices[3 * triangles[3 * i + 1] + 2]);
		//cout << "b: " << b.x << " " << b.y << " " << b.z << endl;
		Vector3 c = Vector3(vertices[3 * triangles[3 * i + 2]], vertices[3 * triangles[3 * i + 2] + 1], vertices[3 * triangles[3 * i + 2] + 2]);
		//cout << "c: " << c.x << " " << c.y << " " << c.z << endl;
		Vector3 n = glm::normalize(glm::cross(b - a, c - a));
		faceNormals.push_back(n);
		//cout << "normal: " << n.x << " " << n.y << " " << n.z << endl;
	}
	for (int i = 0; i < 8; i++)
	{
		std::vector<int> trianglesUsingThisVertex;
		for (int j = 0; j < 3 * 12; j += 3)
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

	GLuint vertexbuffer;
	glGenBuffers(1, &vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(
		0,                  // Атрибут 0. Подробнее об этом будет рассказано в части, посвященной шейдерам.
		3,                  // Размер
		GL_FLOAT,           // Тип
		GL_FALSE,           // Указывает, что значения не нормализованы
		0,                  // Шаг
		(void*)0            // Смещение массива в буфере
	);

	GLuint trianglesbuffer;
	glGenBuffers(1, &trianglesbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, trianglesbuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(trianglesbuffer), triangles, GL_STATIC_DRAW);

	GLuint normalbuffer;
	glGenBuffers(1, &normalbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
	glBufferData(GL_ARRAY_BUFFER, vertexNormals.size() * sizeof(Vector3), &vertexNormals[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(
		2,                                // attribute
		3,                                // size
		GL_FLOAT,                         // type
		GL_FALSE,                         // normalized?
		0,                                // stride
		(void*)0                          // array buffer offset
	);

	// Получить хэндл переменной в шейдере
	// Только один раз во время инициализации.
	GLuint MatrixID = glGetUniformLocation(programID, "MVP");
	GLuint LightDirectionID = glGetUniformLocation(programID, "lightDirection");

	//glEnable(GL_CULL_FACE);
	//glFrontFace(GL_CCW);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//glEnable(GL_DEPTH_TEST);
	//glDepthFunc(GL_ALWAYS);

	Vector3 light(0, 1, 0);

	// render loop
	// -----------
	auto previousTime = std::chrono::high_resolution_clock::now();
	while (!glfwWindowShouldClose(Initializer.window))
	{
		EventSystem::Update();

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(trianglesbuffer), triangles, GL_STATIC_DRAW);
		//glBufferData(GL_ARRAY_BUFFER, vertexNormals.size() * sizeof(Vector3), &vertexNormals[0], GL_STATIC_DRAW);

		/*camera->gameObject->transform->Translate(
			glm::normalize(camera->gameObject->transform->GetForward()) * (0.05f * camera->CheckForwardMotion()) +
			glm::normalize(camera->gameObject->transform->GetRight()) * (0.05f * camera->CheckSideMotion()));*/

		// Матрица модели : единичная матрица (Модель находится в начале координат)
		glm::mat4 Model = glm::mat4(1.0f);  // Индивидуально для каждой модели

		// Итоговая матрица ModelViewProjection, которая является результатом перемножения наших трех матриц
		glm::mat4 MVP = camera->GetProjectionMatrix() * camera->GetViewMatrix() * Model; // Помните, что умножение матрицы производится в обратном порядке

		auto currentTime = std::chrono::high_resolution_clock::now();
		uint64_t deltaTime = std::chrono::duration_cast<std::chrono::nanoseconds>(currentTime - previousTime).count();
		previousTime = currentTime;

		RenderText(
			shader, 
			Vectors::VectorToString(camera->gameObject->transform->GetRotation()), 
			0.0f, 10.0f, 0.75f, Vector3(1.0f, 1.0f, 1.0f));
		glUniformMatrix4fv(glGetUniformLocation(shader.ID, "projection"), 1, GL_FALSE, &projection[0][0]);

		glUseProgram(programID);
		// Передать наши трансформации в текущий шейдер
		// Это делается в основном цикле, поскольку каждая модель будет иметь другую MVP-матрицу (как минимум часть M)
		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
		glUniform3fv(LightDirectionID, 1, &light[0]);

		glBindVertexArray(VertexArrayID);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, triangles);

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