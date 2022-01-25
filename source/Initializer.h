#pragma once
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/vec2.hpp>

#define Initializer InitializationHandler::Instance()

using glm::Vector2;
using glm::Vector3;
using glm::Vector4;
using std::cout;
using std::endl;

class InitializationHandler final
{
private:
	InitializationHandler() {}
	~InitializationHandler() {}
	InitializationHandler(const InitializationHandler&) = delete;
	InitializationHandler& operator = (const InitializationHandler&) = delete;
public:
	GLFWwindow* window;
	const Vector2 windowSize = Vector2(1600, 800);

	static InitializationHandler& Instance()
	{
		static InitializationHandler object; 
		return object;
	}

	float GetAspectRatio()
	{
		return windowSize.x / windowSize.y;
	}

	bool Init()
	{
		if (!glfwInit())
		{
			cout << "glfwInit failed" << endl;
			return false;
		}
		
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

		window = glfwCreateWindow(windowSize.x, windowSize.y, "Game", nullptr, nullptr);
		if (!window)
		{
			glfwTerminate();
			cout << "glfwCreateWindow failed" << endl;
			return false;
		}

		glfwMakeContextCurrent(window);

		if (!gladLoadGL())
		{
			cout << "gladLoadGL failed" << endl;
			return false;
		}

		cout << "Renderer: " << glGetString(GL_RENDERER) << endl;
		cout << "OpenGL Version: " << glGetString(GL_VERSION) << endl;

		return true;
	}

	void Quit()	{ glfwTerminate(); }
};