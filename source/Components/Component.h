#pragma once

#include <GLFW\glfw3.h>
#include "../Math/Vectors.h"

class GameObject;

#define COMPONENT(name) \
friend class GameObject; \
protected: \
explicit name(GameObject* gameObject) : Component(gameObject) { OnCreate(); } \

#define ATTACH_UPDATE \
EventSystem::AttachUpdateEventListener(GetComponentID(), \
[&]() { OnUpdate(); }); \

#define ATTACH_MOUSE_MOVE \
EventSystem::AttachMouseMoveEventListener(GetComponentID(), \
[&](GLFWwindow* window, double x, double y, Vector2 motion) \
	{ OnMouseMove(window, x, y, motion); }); \

#define ATTACH_MOUSE_BUTTON \
EventSystem::AttachMouseButtonEventListener(GetComponentID(), \
[&](GLFWwindow* window, int button, int action, int mods) \
	{ OnMouseClick(window, button, action, mods); }); \

#define ATTACH_MOUSE_WHEEL \
EventSystem::AttachMouseWheelEventListener(GetComponentID(), \
[&](GLFWwindow* window, double xoffset, double yoffset) \
	{ OnMouseWheel(window, xoffset, yoffset); }); \

#define ATTACH_KEYBOARD \
EventSystem::AttachKeyboardEventListener(GetComponentID(), \
[&](GLFWwindow* window, int key, int scancode, int action, int mode) \
	{ OnKeyboardButton(window, key, scancode, action, mode); }); \

#define ATTACH_WINDOW_RESIZE \
EventSystem::AttachWindowResizeEventListener(GetComponentID(), \
[&](GLFWwindow* window, int width, int height) \
	{ OnWindowResize(window, width, height); }); \

#define DETACH_UPDATE \
EventSystem::ReleaseUpdateEventListener(GetComponentID()); \

#define DETACH_MOUSE_MOVE \
EventSystem::ReleaseMouseMoveEventListener(GetComponentID()); \

#define DETACH_MOUSE_BUTTON \
EventSystem::ReleaseMouseButtonEventListener(GetComponentID()); \

#define DETACH_MOUSE_WHEEL \
EventSystem::ReleaseMouseWheelEventListener(GetComponentID()); \

#define DETACH_KEYBOARD \
EventSystem::ReleaseKeyboardEventListener(GetComponentID()); \

#define DETACH_WINDOW_RESIZE \
EventSystem::ReleaseWindowResizeEventListener(GetComponentID()); \

class Component 
{
	friend class GameObject;

protected:
	explicit Component(GameObject* gameObject) : gameObject(gameObject) { OnCreate(); }

	Component(Component& other) = delete;
	Component operator = (Component& other) = delete;

	void operator delete(void* ptr)
	{
		::operator delete(ptr);
	}

	void operator delete[](void* ptr)
	{
		::operator delete[](ptr);
	}

	~Component();

public:
	GameObject* const gameObject;

	long long GetComponentID()
	{
		return reinterpret_cast<long long>(this);
	}

	virtual void OnCreate() {}
	virtual void OnDestroy() {}
	virtual void OnUpdate() {}
	virtual void OnMouseMove(GLFWwindow* window, double x, double y, Vector2 motion) {}
	virtual void OnMouseClick(GLFWwindow* window, int button, int action, int mods) {}
	virtual void OnMouseWheel(GLFWwindow* window, double xoffset, double yoffset) {}
	virtual void OnKeyboardButton(GLFWwindow* window, int key, int scancode, int action, int mode) {}
	virtual void OnWindowResize(GLFWwindow* window, int width, int height) {}
};

class CannotBeRemovedComponentAttribute {};
class MustBeUniqueComponentAttribute {};