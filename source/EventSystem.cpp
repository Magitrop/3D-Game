#pragma once

#include <glad/glad.h>
#include "EventSystem.h"

std::map<long long, std::function<void()>> EventSystem::onUpdate;
std::map<long long, std::function<void(GLFWwindow*, double, double, Vector2)>> EventSystem::onMouseMoveEvent;
std::map<long long, std::function<void(GLFWwindow*, int, int, int)>> EventSystem::onMouseButtonEvent;
std::map<long long, std::function<void(GLFWwindow*, double, double)>> EventSystem::onMouseWheelEvent;
std::map<long long, std::function<void(GLFWwindow*, int, int, int, int)>> EventSystem::onKeyboardEvent;
std::map<long long, std::function<void(GLFWwindow*, int, int)>> EventSystem::onWindowResizeEvent;

Vector2 EventSystem::mousePos;
Vector2 EventSystem::prevMousePos;
Vector2 EventSystem::mouseMotion;

Vector2 EventSystem::mouseWheelOffset;

bool EventSystem::leftMouseButton;
bool EventSystem::rightMouseButton;
std::list<int> EventSystem::pressedKeys;

const Vector2& EventSystem::GetMousePosition()
{
	return mousePos;
}
const Vector2& EventSystem::GetMouseMotion()
{
	return mouseMotion;
}
const Vector2& EventSystem::GetMouseWheelOffset()
{
	return mouseWheelOffset;
}
const std::list<int>& EventSystem::GetPressedKeys()
{
	return pressedKeys;
}
const bool& EventSystem::GetLeftMouseButton()
{
	return leftMouseButton;
}
const bool& EventSystem::GetRightMouseButton()
{
	return rightMouseButton;
}

// Update event
void EventSystem::AttachUpdateEventListener(long long listenerID, std::function<void()> listener)
{
	onUpdate[listenerID] = listener;
}
// Mouse move event
void EventSystem::AttachMouseMoveEventListener(long long listenerID, std::function<void(GLFWwindow*, double, double, Vector2)> listener)
{
	onMouseMoveEvent[listenerID] = listener;
}
// Mouse button event
void EventSystem::AttachMouseButtonEventListener(long long listenerID, std::function<void(GLFWwindow*, int, int, int)> listener)
{
	onMouseButtonEvent[listenerID] = listener;
}
// Mouse wheel event
void EventSystem::AttachMouseWheelEventListener(long long listenerID, std::function<void(GLFWwindow*, double, double)> listener)
{
	onMouseWheelEvent[listenerID] = listener;
}
// Keyboard event
void EventSystem::AttachKeyboardEventListener(long long listenerID, std::function<void(GLFWwindow*, int, int, int, int)> listener)
{
	onKeyboardEvent[listenerID] = listener;
}
// Window resize event
void EventSystem::AttachWindowResizeEventListener(long long listenerID, std::function<void(GLFWwindow*, int, int)> listener)
{
	onWindowResizeEvent[listenerID] = listener;
}

// Update event
void EventSystem::ReleaseUpdateEventListener(long long listenerID)
{
	onUpdate.erase(listenerID);
}
// Mouse move event
void EventSystem::ReleaseMouseMoveEventListener(long long listenerID)
{
	onMouseMoveEvent.erase(listenerID);
}
// Mouse button event
void EventSystem::ReleaseMouseButtonEventListener(long long listenerID)
{
	onMouseButtonEvent.erase(listenerID);
}
// Mouse wheel event
void EventSystem::ReleaseMouseWheelEventListener(long long listenerID)
{
	onMouseWheelEvent.erase(listenerID);
}
// Keyboard event
void EventSystem::ReleaseKeyboardEventListener(long long listenerID)
{
	onKeyboardEvent.erase(listenerID);
}
// Window resize event
void EventSystem::ReleaseWindowResizeEventListener(long long listenerID)
{
	onWindowResizeEvent.erase(listenerID);
}

void EventSystem::Update()
{
	for (auto it = onUpdate.begin(); it != onUpdate.end(); it++)
		it->second();
}
void EventSystem::MouseMoveEvent(GLFWwindow* window, double x, double y)
{
	prevMousePos = mousePos;
	mousePos.x = x;
	mousePos.y = y;
	mouseMotion = prevMousePos - mousePos;
	for (auto it = onMouseMoveEvent.begin(); it != onMouseMoveEvent.end(); it++)
		it->second(window, x, y, mouseMotion);
}
void EventSystem::MouseButtonEvent(GLFWwindow* window, int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_LEFT)
	{
		if (action == GLFW_PRESS)
			leftMouseButton = true;
		else if (action == GLFW_RELEASE)
			leftMouseButton = false;
	}
	else if (button == GLFW_MOUSE_BUTTON_RIGHT)
	{
		if (action == GLFW_PRESS)
			rightMouseButton = true;
		else if (action == GLFW_RELEASE)
			rightMouseButton = false;
	}

	for (auto it = onMouseButtonEvent.begin(); it != onMouseButtonEvent.end(); it++)
		it->second(window, button, action, mods);
}
void EventSystem::MouseWheelEvent(GLFWwindow* window, double xoffset, double yoffset)
{
	mouseWheelOffset.x = xoffset;
	mouseWheelOffset.y = yoffset;
	for (auto it = onMouseWheelEvent.begin(); it != onMouseWheelEvent.end(); it++)
		it->second(window, xoffset, yoffset);
}
void EventSystem::KeyboardEvent(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (action == GLFW_PRESS)
		pressedKeys.push_back(key);
	else if (action == GLFW_RELEASE)
		pressedKeys.erase(std::find(pressedKeys.begin(), pressedKeys.end(), key));

	for (auto it = onKeyboardEvent.begin(); it != onKeyboardEvent.end(); it++)
		it->second(window, key, scancode, action, mode);
}
void EventSystem::WindowResizeEvent(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);

	for (auto it = onWindowResizeEvent.begin(); it != onWindowResizeEvent.end(); it++)
		it->second(window, width, height);
}