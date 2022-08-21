#pragma once
#include "Component.h"

#define _USE_MATH_DEFINES
#include <cmath>
#include <algorithm>

#include <memory>
#include <vector>
#include <GLFW\glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class CameraComponent : public Component,
	MustBeUniqueComponentAttribute
{
	COMPONENT(CameraComponent, Component)
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
	float rotationSpeed = 0.03f;
	float movementSpeed = 0.05f;
	float zoomIntensity = 0.2f;
	Vector2 cameraRestraints = Vector2(0, 80);

	void SetFieldOfView(float fov);
	void SetClippingPlanes(float near, float far);
	const glm::Matrix4x4& GetProjectionMatrix() const;
	const glm::Matrix4x4& GetViewMatrix() const;

	virtual void OnCreate() override;
	virtual void OnUpdate() override;
	virtual void OnMouseMove(GLFWwindow* window, double x, double y, Vector2 motion) override;
	virtual void OnMouseWheel(GLFWwindow* window, double xoffset, double yoffset) override;
	virtual void OnWindowResize(GLFWwindow* window, int width, int height) override;
};