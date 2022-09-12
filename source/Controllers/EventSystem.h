#pragma once

#include <map>
#include <vector>
#include <list>
#include <functional>

#include "../GameObject/GameObject.h"
#include "../Components/CameraComponent.h"

class EventSystem final
{
private:
	EventSystem() = delete;

	static std::map<long long, std::function<void()>> onUpdate;
	static std::map<long long, std::function<void(GLFWwindow*, double, double, Vector2)>> onMouseMoveEvent;
	static std::map<long long, std::function<void(GLFWwindow*, int, int, int)>> onMouseButtonEvent;
	static std::map<long long, std::function<void(GLFWwindow*, double, double)>> onMouseWheelEvent;
	static std::map<long long, std::function<void(GLFWwindow*, int, int, int, int)>> onKeyboardEvent;
	static std::map<long long, std::function<void(GLFWwindow*, int, int)>> onWindowResizeEvent;

	static Vector2 mousePos;
	static Vector2 prevMousePos;
	static Vector2 mouseMotion;

	static Vector2 mouseWheelOffset;

	static bool leftMouseButtonLastFrame;
	static bool rightMouseButtonLastFrame;
	static bool leftMouseButton;
	static bool rightMouseButton;
	static bool keys[GLFW_KEY_LAST];

	static CameraComponent* mainCamera;
public:
	static const Vector2& GetMousePosition();
	static const Vector2& GetMouseMotion();
	static const Vector2& GetMouseWheelOffset();
	static const bool GetKey(int keyCode);
	static const bool GetMouseButton(int button);
	static const bool GetMouseButtonClick(int button);
	static void SetAsMainCamera(CameraComponent* cam);
	static CameraComponent* const GetMainCamera();

	// Update event
	static void AttachUpdateEventListener(long long listenerID, std::function<void()> listener);
	// Mouse move event
	static void AttachMouseMoveEventListener(long long listenerID, std::function<void(GLFWwindow*, double, double, Vector2)> listener);
	// Mouse button event
	static void AttachMouseButtonEventListener(long long listenerID, std::function<void(GLFWwindow*, int, int, int)> listener);
	// Mouse wheel event
	static void AttachMouseWheelEventListener(long long listenerID, std::function<void(GLFWwindow*, double, double)> listener);
	// Keyboard event
	static void AttachKeyboardEventListener(long long listenerID, std::function<void(GLFWwindow*, int, int, int, int)> listener);
	// Window resize event
	static void AttachWindowResizeEventListener(long long listenerID, std::function<void(GLFWwindow*, int, int)> listener);

	// Update event
	static void ReleaseUpdateEventListener(long long listenerID);
	// Mouse move event
	static void ReleaseMouseMoveEventListener(long long listenerID);
	// Mouse button event
	static void ReleaseMouseButtonEventListener(long long listenerID);
	// Mouse wheel event
	static void ReleaseMouseWheelEventListener(long long listenerID);
	// Keyboard event
	static void ReleaseKeyboardEventListener(long long listenerID);
	// Window resize event
	static void ReleaseWindowResizeEventListener(long long listenerID);

	static void Update();
	static void MouseMoveEvent(GLFWwindow* window, double x, double y);
	static void MouseButtonEvent(GLFWwindow* window, int button, int action, int mods);
	static void MouseWheelEvent(GLFWwindow* window, double xoffset, double yoffset);
	static void KeyboardEvent(GLFWwindow* window, int key, int scancode, int action, int mode);
	static void WindowResizeEvent(GLFWwindow* window, int width, int height);
};