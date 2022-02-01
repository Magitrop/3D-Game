#pragma once
#include "../Initializer.h"

#define _USE_MATH_DEFINES
#include <cmath>
#include <algorithm>

#include <memory>
#include <vector>
#include <GLFW\glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "../GameObject/ObjectsManager.h"
#include "TransformComponent.h"
#include "../EventSystem.h"

class CameraComponent : public Component,
	MustBeUniqueComponentAttribute
{
	COMPONENT(CameraComponent)
protected:
	glm::Matrix4x4 projectionMatrix;
	glm::Matrix4x4 viewMatrix;

	std::vector<int> sideMotionOverrides;		// right or left
	std::vector<int> forwardMotionOverrides;	// forward or backward

	Vector3 newRotation = Vector3();
	Vector3 targetPoint = Vector3();
	Vector2 movementDirection = Vector2();

	float fieldOfView = 45.f;
	float nearClipPlane = 0.01f;
	float farClipPlane = 100.f;

	//bool freeCamera = false;

	void RecalculateProjectionMatrix();
	void RecalculateViewMatrix();
public:
	float rotationSpeed = 0.001f;
	float movementSpeed = 0.05f;
	float zoomIntensity = 0.2f;
	Vector2 cameraRestraints = Vector2(glm::radians(0.f), glm::radians(80.f));

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

	void SetFieldOfView(float fov);
	void SetClippingPlanes(float near, float far);
	const glm::Matrix4x4& GetProjectionMatrix() const;
	const glm::Matrix4x4& GetViewMatrix() const;

	virtual void OnCreate() override;
	virtual void OnUpdate() override;
	virtual void OnMouseMove(GLFWwindow* window, double x, double y, Vector2 motion) override;
	virtual void OnMouseWheel(GLFWwindow* window, double xoffset, double yoffset) override;
	virtual void OnWindowResize(GLFWwindow* window, int width, int height) override;
	virtual void OnKeyboardButton(GLFWwindow* window, int key, int scancode, int action, int mode) override;
};