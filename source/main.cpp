﻿#define _USE_MATH_DEFINES
#include <cmath>

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
#include "Math/Vectors.h"
#include "GameObject/GameObject.h"
#include "Shader.h"

using glm::Matrix4x4;

Vector2 mousePos;
Vector2 prevMousePos;
Vector2 mouseMotion;
bool lmb, rmb;

float cameraRotationSpeed = 0.001f;
float cameraMovementSpeed = 0.05f;
Vector3 cameraRotation = Vector3();
Vector3 cameraPos = Vector3(0, 0, 3);

struct Camera
{
	Vector3 forward = Vector3();
	Vector3 right = Vector3();
	Vector3 up = Vector3();

	std::vector<int> sideMotionOverrides;		// right or left
	std::vector<int> forwardMotionOverrides;	// forward or backward

	void OverrideMotion(int key, bool addKey)
	{
		switch (key)
		{
		case GLFW_KEY_A:
		case GLFW_KEY_D:
		{
			auto
				it = sideMotionOverrides.begin(),
				lim = sideMotionOverrides.end();
			for (; it != lim; ++it)
				if (*it == key)
				{
					if (addKey)
						std::rotate(sideMotionOverrides.begin(), it, it + 1);
					else
						sideMotionOverrides.erase(it);
					return;
				}
			if (addKey)
				sideMotionOverrides.insert(sideMotionOverrides.begin(), key);
			return;
		}
		case GLFW_KEY_W:
		case GLFW_KEY_S:
		{
			auto
				it = forwardMotionOverrides.begin(),
				lim = forwardMotionOverrides.end();
			for (; it != lim; ++it)
				if (*it == key)
				{
					if (addKey)
						std::rotate(forwardMotionOverrides.begin(), it, it + 1);
					else
						forwardMotionOverrides.erase(it);
					return;
				}
			if (addKey)
				forwardMotionOverrides.insert(forwardMotionOverrides.begin(), key);
			return;
		}
		}
	}

	int CheckSideMotion()
	{
		if (sideMotionOverrides.size() == 0)
			return 0;
		if (sideMotionOverrides[0] == GLFW_KEY_A)
			return -1;
		if (sideMotionOverrides[0] == GLFW_KEY_D)
			return 1;
	}

	int CheckForwardMotion()
	{
		if (forwardMotionOverrides.size() == 0)
			return 0;
		if (forwardMotionOverrides[0] == GLFW_KEY_S)
			return -1;
		if (forwardMotionOverrides[0] == GLFW_KEY_W)
			return 1;
	}
} camera;

Matrix4x4 CreateRotationX(float angle)
{
	Matrix4x4 newMatrix = Matrix4x4();
	newMatrix[1][1] = std::cos(-angle);
	newMatrix[1][2] = -std::sin(-angle);
	newMatrix[2][1] = std::sin(-angle);
	newMatrix[2][2] = std::cos(-angle);
	return newMatrix;
}
Matrix4x4 CreateRotationY(float angle)
{
	Matrix4x4 newMatrix = Matrix4x4();
	newMatrix[0][0] = std::cos(-angle);
	newMatrix[0][2] = std::sin(-angle);
	newMatrix[2][0] = -std::sin(-angle);
	newMatrix[2][2] = std::cos(-angle);
	return newMatrix;
}
Matrix4x4 CreateRotationZ(float angle)
{
	Matrix4x4 newMatrix = Matrix4x4();
	newMatrix[0][0] = std::cos(-angle);
	newMatrix[0][1] = -std::sin(-angle);
	newMatrix[1][0] = std::sin(-angle);
	newMatrix[1][1] = std::cos(-angle);
	return newMatrix;
}
Vector3 MultiplyPoint(Matrix4x4 m, Vector3 p)
{
	Vector3 vector3;
	vector3.x = (m[0][0] * p.x + m[0][1] * p.y + m[0][2] * p.z) + m[0][3];
	vector3.y = (m[1][0] * p.x + m[1][1] * p.y + m[1][2] * p.z) + m[1][3];
	vector3.z = (m[2][0] * p.x + m[2][1] * p.y + m[2][2] * p.z) + m[2][3];
	return vector3;
}

void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_W || 
		key == GLFW_KEY_A || 
		key == GLFW_KEY_S || 
		key == GLFW_KEY_D)
	{
		if (action == GLFW_PRESS)
			camera.OverrideMotion(key, true);
		else if (action == GLFW_RELEASE)
			camera.OverrideMotion(key, false);
	}
}

void CursorPosCallback(GLFWwindow* window, double x, double y)
{
	prevMousePos = mousePos;
	mousePos.x = x;
	mousePos.y = y;
	mouseMotion = prevMousePos - mousePos;
}

void MouseClickCallback(GLFWwindow* window, int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_LEFT)
	{
		if (action == GLFW_PRESS)
			lmb = true;
		else if (action == GLFW_RELEASE)
			lmb = false;
	}
	else if (button == GLFW_MOUSE_BUTTON_RIGHT)
	{
		if (action == GLFW_PRESS)
		{
			glfwSetInputMode(Initializer.window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
			rmb = true;
		}
		else if (action == GLFW_RELEASE)
		{
			glfwSetInputMode(Initializer.window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
			rmb = false;
		}
	}
}

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
	int InfoLogLength;

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

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void RenderText(Shader& shader, std::string text, float x, float y, float scale, glm::vec3 color);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

/// Holds all state information relevant to a character as loaded using FreeType
struct Character {
	unsigned int TextureID; // ID handle of the glyph texture
	glm::ivec2   Size;      // Size of glyph
	glm::ivec2   Bearing;   // Offset from baseline to left/top of glyph
	unsigned int Advance;   // Horizontal offset to advance to next glyph
};

std::map<GLchar, Character> Characters;
unsigned int VAO, VBO;

int main()
{
	// glfw: initialize and configure
	// ------------------------------
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// glfw window creation
	// --------------------
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// OpenGL state
	// ------------
	glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// compile and setup the shader
	// ----------------------------
	Shader shader("..\\source\\TextVertexShader.vertexshader", "..\\source\\TextFragmentShader.fragmentshader");
	glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(SCR_WIDTH), 0.0f, static_cast<float>(SCR_HEIGHT));
	shader.Use();
	glUniformMatrix4fv(glGetUniformLocation(shader.ID, "projection"), 1, GL_FALSE, &projection[0][0]);

	FT_Library ft;
	if (FT_Init_FreeType(&ft))
	{
		std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
		return -1;
	}

	FT_Face face;
	if (FT_New_Face(ft, "../fonts/ekaterinavelikayatwo.ttf", 0, &face))
	{
		std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
		return -1;
	}
	else 
	{
		// set size to load glyphs as
		FT_Set_Pixel_Sizes(face, 0, 128);

		// disable byte-alignment restriction
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

		// load first 128 characters of ASCII set
		for (unsigned char c = 0; c < 128; c++)
		{
			// Load character glyph 
			if (FT_Load_Char(face, c, FT_LOAD_RENDER))
			{
				std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
				continue;
			}
			// generate texture
			unsigned int texture;
			glGenTextures(1, &texture);
			glBindTexture(GL_TEXTURE_2D, texture);
			glTexImage2D(
				GL_TEXTURE_2D,
				0,
				GL_RED,
				face->glyph->bitmap.width,
				face->glyph->bitmap.rows,
				0,
				GL_RED,
				GL_UNSIGNED_BYTE,
				face->glyph->bitmap.buffer
			);
			// set texture options
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			// now store character for later use
			Character character = {
				texture,
				glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
				glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
				static_cast<unsigned int>(face->glyph->advance.x)
			};
			Characters.insert(std::pair<char, Character>(c, character));
		}
		glBindTexture(GL_TEXTURE_2D, 0);
	}
	// destroy FreeType once we're finished
	FT_Done_Face(face);
	FT_Done_FreeType(ft);


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

	// render loop
	// -----------
	while (!glfwWindowShouldClose(window))
	{
		// input
		// -----
		processInput(window);

		// render
		// ------
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		RenderText(shader, "This is sample text", 25.0f, 25.0f, 1.0f, glm::vec3(0.5, 0.8f, 0.2f));
		RenderText(shader, "(C) LearnOpenGL.com", 540.0f, 570.0f, 0.5f, glm::vec3(0.3, 0.7f, 0.9f));

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}


// render line of text
// -------------------
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
		Character ch = Characters[*c];

		float xpos = x + ch.Bearing.x * scale;
		float ypos = y - (ch.Size.y - ch.Bearing.y) * scale;

		float w = ch.Size.x * scale;
		float h = ch.Size.y * scale;
		// update VBO for each character
		float vertices[6][4] = {
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

/*unsigned int VAO, VBO;
void RenderText(Shader& s, std::string text, float x, float y, float scale, glm::vec3 color)
{
	// activate corresponding render state	
	s.Use();
	glUniform3f(glGetUniformLocation(s.ID, "textColor"), color.x, color.y, color.z);
	glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(VAO);

	// iterate through all characters
	for (auto c = text.begin(); c != text.end(); c++)
	{
		Character ch = Characters[*c];

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
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		// render quad
		glDrawArrays(GL_TRIANGLES, 0, 6);
		// now advance cursors for next glyph (note that advance is number of 1/64 pixels)
		x += (ch.Advance >> 6) * scale; // bitshift by 6 to get value in pixels (2^6 = 64)
	}
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

int main(int argc, char* argv[])
{
	auto go = ObjectsManager::Instantiate<TransformComponent>();
	go->gameObject->AddComponent<SampleComponent>();
	go->gameObject->RemoveComponent<SampleComponent>();
	go->gameObject->AddComponent<SampleComponent>();

	if (!Initializer.Init())
		return -1;

	glfwSetKeyCallback(Initializer.window, KeyCallback);
	glfwSetCursorPosCallback(Initializer.window, CursorPosCallback);
	glfwSetMouseButtonCallback(Initializer.window, MouseClickCallback);

	auto previousTime = std::chrono::high_resolution_clock::now();

	// Создать и откомпилировать нашу шейдерную программу
	/*GLuint programID = LoadShaders(
		"..\\source\\SimpleVertexShader.vertexshader",
		"..\\source\\SimpleFragmentShader.fragmentshader");*/

	/*GLuint VertexArrayID;
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

	cameraRotation.y = 0;
	//glEnable(GL_CULL_FACE);
	//glFrontFace(GL_CCW);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//glEnable(GL_DEPTH_TEST);
	//glDepthFunc(GL_ALWAYS);

	Vector3 light(0, 1, 0);

	glm::mat4 textProjection = glm::ortho(0.0f, Initializer.windowSize.x, 0.0f, Initializer.windowSize.y);
	Shader shader("..\\source\\TextVertexShader.vertexshader", "..\\source\\TextFragmentShader.fragmentshader");
	glUniformMatrix4fv(glGetUniformLocation(shader.ID, "projection"), 1, GL_FALSE, &textProjection[0][0]);

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(
		0, 
		4, 
		GL_FLOAT, 
		GL_FALSE, 
		4 * sizeof(float), 
		(void*)0
	);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	while (!glfwWindowShouldClose(Initializer.window))
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		RenderText(shader, "This is sample text", 25.0f, 25.0f, 1.0f, glm::vec3(1.0f, 1.0f, 1.0f));

		//glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(trianglesbuffer), triangles, GL_STATIC_DRAW);
		//glBufferData(GL_ARRAY_BUFFER, vertexNormals.size() * sizeof(Vector3), &vertexNormals[0], GL_STATIC_DRAW);

		if (rmb)
		{
			cameraRotation.x -= mouseMotion.y * cameraRotationSpeed;
			//cameraRotation.x = std::clamp(cameraRotation.x, 0.0f, glm::radians(80.0f));

			cameraRotation.y -= mouseMotion.x * cameraRotationSpeed;
		}

		// Проекционная матрица : 45deg поле обзора, 4:3 соотношение сторон, диапазон : 0.1 юнит <-> 100 юнитов
		glm::mat4 Projection = glm::perspective(glm::radians(45.0f), Initializer.GetAspectRatio(), 0.01f, 100.0f);

		camera.forward.x = sin(cameraRotation.y) * cos(cameraRotation.x);
		camera.forward.y = -sin(cameraRotation.x);
		camera.forward.z = -cos(cameraRotation.y) * cos(cameraRotation.x);

		camera.forward = glm::normalize(-cameraPos);

		camera.right.x = cos(cameraRotation.y);
		camera.right.y = 0.0;
		camera.right.z = sin(cameraRotation.y);

		camera.up = glm::cross(-camera.forward, camera.right);

		cameraPos += 
			glm::normalize(camera.forward) * (cameraMovementSpeed * camera.CheckForwardMotion()) +
			glm::normalize(camera.right) * (cameraMovementSpeed * camera.CheckSideMotion());

		auto cam = MultiplyPoint(glm::rotate(Matrix4x4(1), cameraRotation.y, Vector3(0, 1, 0)), cameraPos);
		cam = MultiplyPoint(glm::rotate(Matrix4x4(1), cameraRotation.x, camera.right), cam);

		// Или, для ортокамеры
		glm::mat4 View = glm::lookAt(
			cam, // Камера находится в мировых координатах
			Vector3(0, 0, 0)/*cameraPos + camera.forward, // И направлена вперед
			glm::vec3(0, 1, 0)  // "Голова" находится сверху
		);
		
		// Матрица модели : единичная матрица (Модель находится в начале координат)
		glm::mat4 Model = glm::mat4(1.0f);  // Индивидуально для каждой модели

		// Итоговая матрица ModelViewProjection, которая является результатом перемножения наших трех матриц
		glm::mat4 MVP = Projection * View * Model; // Помните, что умножение матрицы производится в обратном порядке

		auto currentTime = std::chrono::high_resolution_clock::now();
		uint64_t deltaTime = std::chrono::duration_cast<std::chrono::nanoseconds>(currentTime - previousTime).count();
		previousTime = currentTime;

		//glUseProgram(programID);
		// Передать наши трансформации в текущий шейдер
		// Это делается в основном цикле, поскольку каждая модель будет иметь другую MVP-матрицу (как минимум часть M)
		//glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
		//glUniform3fv(LightDirectionID, 1, &light[0]);

		//glBindVertexArray(VertexArrayID);
		//glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, triangles);

		//game.update(deltaTime);
		//game.render();

		for (long long i = 0; i < 1999999; ++i);

		glfwSwapBuffers(Initializer.window);

		mouseMotion.x = 0;
		mouseMotion.y = 0;
		glfwPollEvents();
	}

	//resources.deleteAllResources();
	Initializer.Quit();
	return 0;
}*/