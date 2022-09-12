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
	projectionViewMatrix = projectionMatrix * viewMatrix;
	projectionViewMatrixInverted = glm::inverse(projectionMatrix * viewMatrix);
}
void CameraComponent::RecalculateViewMatrix()
{
	viewMatrix =
		glm::lookAt(
			gameObject->transform->GetPosition(),
			targetPoint,//cameraPos + camera->forward,
			Vectors::up
		);
	projectionViewMatrix = projectionMatrix * viewMatrix;
	projectionViewMatrixInverted = glm::inverse(projectionMatrix * viewMatrix);
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
const glm::Matrix4x4& CameraComponent::GetProjectionViewMatrix() const
{
	return projectionViewMatrix;
}

const glm::Matrix4x4& CameraComponent::GetInvertedProjectionViewMatrix() const
{
	return projectionViewMatrixInverted;
}

Ray CameraComponent::ScreenPointToRay(Vector2 screenPoint) const
{
	Vector4 point = Vectors::AsVector4(screenPoint);
	Matrix4x4 invMat = GetInvertedProjectionViewMatrix();
	Vector4 near(2.f * point.x / Initializer.windowSize.x - 1.f, 1.f - 2.f * point.y / Initializer.windowSize.y, -1, 1.0);
	Vector4 far(2.f * point.x / Initializer.windowSize.x - 1.f, 1.f - 2.f * point.y / Initializer.windowSize.y, 1, 1.0);
	Vector4 nearResult = invMat * near;
	Vector4 farResult = invMat * far;
	nearResult /= nearResult.w;
	farResult /= farResult.w;
	return Ray(gameObject->transform->GetPosition(), glm::normalize(Vector3(farResult - nearResult)));
}

Vector3 CameraComponent::ScreenToWorldPoint(Vector2 screenPoint) const
{
	Vector4 point = Vectors::AsVector4(screenPoint);
	Matrix4x4 invMat = glm::inverse(GetProjectionViewMatrix());
	Vector4 near(2 * point.x / Initializer.windowSize.x - 1.f, 1.f - 2 * point.y / (Initializer.windowSize.y), -1, 1.0);
	Vector4 far(2 * point.x / Initializer.windowSize.x - 1.f, 1.f - 2 * point.y / (Initializer.windowSize.y), 1, 1.0);
	Vector4 nearResult = invMat * near;
	Vector4 farResult = invMat * far;
	nearResult /= nearResult.w;
	farResult /= farResult.w;
	return gameObject->transform->GetPosition() + glm::normalize(Vector3(farResult - nearResult));
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
	if (EventSystem::GetMouseButton(GLFW_MOUSE_BUTTON_RIGHT))
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
	RecalculateViewMatrix();
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

	if (movementDirection.x || movementDirection.y)
	{
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
}