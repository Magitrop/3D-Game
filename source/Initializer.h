#pragma once

#include <iostream>
#include <map>
#include <GLFW/glfw3.h>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

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

	static InitializationHandler& Instance();

	float GetAspectRatio();
	bool Init();
	void Quit();
};