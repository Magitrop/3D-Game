#include "CameraComponent.h"
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
			targetPoint/*cameraPos + camera->forward*/,
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
	ATTACH_KEYBOARD
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
				gameObject->transform->GetLocalRotation().x - newRotation.x,
				gameObject->transform->GetRight()),
			gameObject->transform->GetPosition() - targetPoint);
		cam = targetPoint + Vectors::MultiplyPoint(
			glm::rotate(
				Matrix4x4(1),
				newRotation.y - gameObject->transform->GetLocalRotation().y,
				Vectors::up),
			cam - targetPoint);

		// applying rotation
		gameObject->transform->SetLocalRotation(newRotation.x, newRotation.y, 0);
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
	gameObject->transform->Translate(gameObject->transform->GetForward() * yoffset * zoomIntensity);
}

void CameraComponent::OnUpdate()
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

void CameraComponent::OnKeyboardButton(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_W)
	{
		if (action == GLFW_PRESS)
			movementDirection.y = 1;
		else if (action == GLFW_RELEASE)
			movementDirection.y = 0;
	}
	else if (key == GLFW_KEY_S)
	{
		if (action == GLFW_PRESS)
			movementDirection.y = -1;
		else if (action == GLFW_RELEASE)
			movementDirection.y = 0;
	}
	else if (key == GLFW_KEY_D)
	{
		if (action == GLFW_PRESS)
			movementDirection.x = -1;
		else if (action == GLFW_RELEASE)
			movementDirection.x = 0;
	}
	else if (key == GLFW_KEY_A)
	{
		if (action == GLFW_PRESS)
			movementDirection.x = 1;
		else if (action == GLFW_RELEASE)
			movementDirection.x = 0;
	}
	/*if (key == GLFW_KEY_W ||
		key == GLFW_KEY_A ||
		key == GLFW_KEY_S ||
		key == GLFW_KEY_D)
	{
		if (action == GLFW_PRESS)
			OverrideMotion(key, true);
		else if (action == GLFW_RELEASE)
			OverrideMotion(key, false);
	}*/
}