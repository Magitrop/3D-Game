#pragma once

#ifdef __gl_h_
#undef __gl_h_
#endif // __gl_h_
#include <glad/glad.h>

#include <iostream>
#include <GLFW/glfw3.h>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

#include "Initializer.h"
#include "../Components/TextRendererComponent.h"
#include "EventSystem.h"

InitializationHandler& InitializationHandler::Instance()
{
	static InitializationHandler object;
	return object;
}

float InitializationHandler::GetAspectRatio()
{
	return windowSize.x / windowSize.y;
}

bool InitializationHandler::Init()
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

	if (!TextHandler::InitializeCharacters())
		return false;

	// glad: load all OpenGL function pointers×
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return false;
	}

	glfwSetKeyCallback(window, EventSystem::KeyboardEvent);
	glfwSetCursorPosCallback(window, EventSystem::MouseMoveEvent);
	glfwSetMouseButtonCallback(window, EventSystem::MouseButtonEvent);
	glfwSetScrollCallback(window, EventSystem::MouseWheelEvent);
	glfwSetFramebufferSizeCallback(window, EventSystem::WindowResizeEvent);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	return true;
}

void InitializationHandler::Quit() 
{ 
	glfwTerminate(); 
}