#pragma once
#include "CameraComponent.h"
#include "../Controllers/Initializer.h"

#include "TransformComponent.h"
#include "../GameObject/ObjectsManager.h"
#include "../Controllers/EventSystem.h"

#include <glm/gtx/projection.hpp>

void CameraComponent::RecalculateProjectionMatrix()
{
	projectionMatrix =
		glm::perspective(
			glm::radians(fieldOfView),
			Initializer.GetAspectRatio(),
			nearClipPlane,
			farClipPlane);
}
void CameraComponent::RecalculateViewMatrix()
{
	viewMatrix =
		glm::lookAt(
			gameObject->transform->GetPosition(),
			targetPoint,//cameraPos + camera->forward,
			Vectors::up
		);
}

void CameraComponent::SetFieldOfView(float fov)
{
	fieldOfView = fov;
	RecalculateProjectionMatrix();
}
void CameraComponent::SetClippingPlanes(float near, float far)
{
	nearClipPlane = near;
	farClipPlane = far;
	RecalculateProjectionMatrix();
}
const glm::Matrix4x4& CameraComponent::GetProjectionMatrix() const
{
	return projectionMatrix;
}
const glm::Matrix4x4& CameraComponent::GetViewMatrix() const
{
	return viewMatrix;
}

void CameraComponent::OnCreate()
{
	ATTACH_MOUSE_MOVE
	ATTACH_UPDATE
	ATTACH_WINDOW_RESIZE
	ATTACH_MOUSE_WHEEL

	RecalculateProjectionMatrix();
	RecalculateViewMatrix();
}

void CameraComponent::OnMouseMove(GLFWwindow* window, double x, double y, Vector2 motion)
{
	if (EventSystem::GetRightMouseButton())
	{
		// translating mouse motion into rotation
		newRotation.x -= EventSystem::GetMouseMotion().y * rotationSpeed;
		newRotation.x = std::clamp(newRotation.x, cameraRestraints.x, cameraRestraints.y);
		newRotation.y -= EventSystem::GetMouseMotion().x * rotationSpeed;

		// rotating camera position
		auto cam = targetPoint + Vectors::MultiplyPoint(
			glm::rotate(
				Matrix4x4(1),
				glm::radians(gameObject->transform->GetRotation().x - newRotation.x),
				gameObject->transform->GetRight()),
			gameObject->transform->GetPosition() - targetPoint);
		cam = targetPoint + Vectors::MultiplyPoint(
			glm::rotate(
				Matrix4x4(1),
				glm::radians(newRotation.y - gameObject->transform->GetRotation().y),
				Vectors::up),
			cam - targetPoint);

		// applying rotation
		gameObject->transform->SetRotation(newRotation.x, newRotation.y, 0);
		gameObject->transform->SetPosition(cam);
		gameObject->transform->SetForward(targetPoint - cam);

		RecalculateViewMatrix();

		glfwSetInputMode(Initializer.window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	}
	else
		glfwSetInputMode(Initializer.window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}

void CameraComponent::OnWindowResize(GLFWwindow* window, int width, int height)
{

}

void CameraComponent::OnMouseWheel(GLFWwindow* window, double xoffset, double yoffset)
{
	gameObject->transform->Translate(gameObject->transform->GetForward() * static_cast<float>(yoffset) * zoomIntensity);
}

void CameraComponent::OnUpdate()
{
	if (EventSystem::GetKey(GLFW_KEY_W))
		movementDirection.y = 1;
	else if (EventSystem::GetKey(GLFW_KEY_S))
		movementDirection.y = -1;
	else 
		movementDirection.y = 0;

	if (EventSystem::GetKey(GLFW_KEY_D))
		movementDirection.x = -1;
	else if (EventSystem::GetKey(GLFW_KEY_A))
		movementDirection.x = 1;
	else
		movementDirection.x = 0;

	targetPoint += 
		glm::normalize(glm::proj(
			gameObject->transform->GetForward(),
			glm::cross(Vectors::up, gameObject->transform->GetRight()))) * movementDirection.y * movementSpeed +
		gameObject->transform->GetRight() * movementDirection.x * movementSpeed;
	gameObject->transform->Translate(
		glm::normalize(glm::proj(
			gameObject->transform->GetForward(),
			glm::cross(Vectors::up, gameObject->transform->GetRight()))) * movementDirection.y * movementSpeed +
		gameObject->transform->GetRight() * movementDirection.x * movementSpeed);
	RecalculateViewMatrix();
}